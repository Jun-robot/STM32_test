#include "mbed.h"

static DigitalOut led(PC_13);
static DigitalOut led2(PC_14);

// static BufferedSerial serial(D8, D2);
static BufferedSerial ToMD(PC_12, PD_2);

static DigitalIn toggle(PA_12);

const int MOTOR_DEG[] = {60, 135, 225, 300}; // モーター配置角度
int out_speed[4];                            //出力値

static DigitalOut MUXs[] = {
    DigitalOut(PC_2), //  配列の1番目の要素をLED4で初期化したDigitalOutに
    DigitalOut(PC_3), //  ..
    DigitalOut(PC_4), //  ..
};

static AnalogIn OUT_X(PC_0);
static AnalogIn OUT_Y(PC_1);

// static BufferedSerial serial(PA_2, PA_3);

void F_MD_rotate2(int LINEIR, int GYRO, int SPEED);
void F_speed_send(int id, int mySpeed);
int F_MD_gyro(int gyro_deg, int myspeed);
long map(long x, long in_min, long in_max, long out_min, long out_max);

int F_IR_mawarikomi_mae(int IR, int leng);
int F_IR_mawarikomi(int IR, int leng);
int F_IR_wrap_around(int IR);

int main(void) {
  toggle.mode(PullDown);
  ToMD.set_baud(57600);
  ToMD.set_format(
      /* bits */ 8, /* parity */ BufferedSerial::None, /* stop bit */ 1);

  led = 0;
  led2 = 1;

  int degree;
  int analog_value[16];
  int duration[16];
  int pre_duration[16];

  const float sine[] = {
      0, 0.38268,  0.70711,  0.92388,  1,  0.92388,  0.70711,  0.38268,
      0, -0.38268, -0.70711, -0.92388, -1, -0.92388, -0.70711, -0.38268};
  const float cosine[] = {
      1,  0.92388,  0.70711,  0.38268,  0, -0.38268, -0.70711, -0.92388,
      -1, -0.92388, -0.70711, -0.38268, 0, 0.38268,  0.70711,  0.92388};

  while (true) {
    int best_id = 0;
    int best_duration = 0;

    for (int j = 0; j < 8; j++) {
      for (int i = 0; i < 3; i++) {
        MUXs[i] = (j >> i) & 0x1;
      }
      analog_value[j] = 60 - (OUT_X.read_u16() / 1000);
      analog_value[j + 8] = 60 - (OUT_Y.read_u16() / 1000);
    }

    for (int id = 0; id < 16; id++) {
      duration[id] = analog_value[id] * 0.6 + pre_duration[id] * 0.4;
      // if (id == 0) {
      //   duration[0] *= 1.12;
      // }
      pre_duration[id] = duration[id];

      best_duration = max(best_duration, duration[id]);
      if (best_duration == duration[id]) {
        best_id = id;
      }

      // printf("%d ",duration[id]);
    }
    duration[7] = (duration[6] + duration[8]) / 2;
    duration[15] = (duration[0] + duration[14]) / 2;

    for (int id = 0; id < 16; id++) {
      printf("%d ", duration[id]);
    }

    if (best_duration > 0) {
      //ボールがあったとき
      int read_id[5];
      int x_axis = 0;
      int y_axis = 0;
      read_id[0] = best_id - 2;
      read_id[1] = best_id - 1;
      read_id[2] = best_id;
      read_id[3] = best_id + 1;
      read_id[4] = best_id + 2;
      for (int i = 0; i < 5; i++) {
        if (read_id[i] > 15) {
          read_id[i] -= 16;
        }
        if (read_id[i] < 0) {
          read_id[i] += 16;
        }

        x_axis += duration[read_id[i]] * cosine[read_id[i]];
        y_axis += duration[read_id[i]] * sine[read_id[i]];
      }

      degree = (atan2((y_axis), (x_axis)) * 180 / 3.1415);
      if (degree > 360) {
        degree -= 360;
      } else if (degree < 1) {
        degree += 360;
      }

    } else {
      //ボールがなかった時
      degree = 0;
    }

    if (toggle == 0) {
      F_MD_rotate2(degree, 1, 450);
    } else {
      F_MD_rotate2(0, 1, 0);
    }
  }
}

//------------------------------------------------
void F_MD_rotate2(int LINEIR, int GYRO, int SPEED) {
  /*
    LINEIR 進行角度(0:無動作,1-360:進む)
    GYRO   目標傾き角度(1-360)
    SPEED  加減速(0-254) 進行角度に係る
  */
  int gyro_speed = F_MD_gyro(GYRO, SPEED);
  int lineIr_speed[4];

  if (LINEIR != 0) {
    float sine_power[4];
    float big_power = 0;

    for (int i = 0; i < 4; i++) {
      int sine_thita = MOTOR_DEG[i] - LINEIR;
      sine_power[i] = (sin(sine_thita * 3.1415 / 180));

      //ジャイロの値を追加する。
      sine_power[i] =
          sine_power[i] + ((gyro_speed / 255 * abs(sine_power[i])) * 0.5);
      //ここまで

      big_power = max(big_power, abs(sine_power[i]));
    }
    float aaa = 1 / big_power;
    for (int i = 0; i < 4; i++) {
      lineIr_speed[i] = ((sine_power[i] * aaa)) * SPEED;
    }
  } else if (gyro_speed != 0) {
    for (int i = 0; i < 4; i++) {
      lineIr_speed[i] = gyro_speed;
    }
  } else {
    for (int i = 0; i < 4; i++) {
      lineIr_speed[i] = 0;
    }
  }

  for (int i = 0; i < 4; i++) {
    out_speed[i] = lineIr_speed[i];
  }

  for (int i = 0; i < 4; i++) {
    if (out_speed[i] == 0) {
      out_speed[i] = 0;
    } else if (out_speed[i] > 0) {
      out_speed[i] =
          out_speed[i] < 40 ? out_speed[i] = 40 : out_speed[i] = out_speed[i];
      out_speed[i] =
          out_speed[i] > 255 ? out_speed[i] = 255 : out_speed[i] = out_speed[i];
    } else if (out_speed[i] < 0) {
      out_speed[i] = out_speed[i] < -255 ? out_speed[i] = -255
                                         : out_speed[i] = out_speed[i];
      out_speed[i] =
          out_speed[i] > -40 ? out_speed[i] = -40 : out_speed[i] = out_speed[i];
    }
    F_speed_send(i, out_speed[i]);
  }
}

int F_MD_gyro(int gyro_deg, int myspeed) {
  /*
    返り値：80-250
    それぞれのモーターごとに計算してね！！
  */
  int pwm_val = 0;
  myspeed = 255;
  if (gyro_deg != 0) {
    if (4 <= gyro_deg && gyro_deg <= 180) {
      //左回り
      pwm_val = map(gyro_deg, 4, 180, 40, myspeed);

    } else if (180 <= gyro_deg && gyro_deg <= 356) {
      //右回り
      pwm_val = map(gyro_deg, 356, 180, -40, -myspeed);
    } else {
      //回らない
      pwm_val = 0;
    }
  } else {
    pwm_val = 0;
  }
  return pwm_val;
}

void F_speed_send(int id, int mySpeed) {
  char a[1] = {0};
  if (mySpeed == -1) {
    //ブレーキ 0
    a[0] = 0 + id;
    ToMD.write(a, 1);
  } else if (mySpeed > 0) {
    //正転 10
    a[0] = 10 + id;
    ToMD.write(a, 1);
    a[0] = mySpeed;
    ToMD.write(a, 1);
  } else if (mySpeed < 0) {
    //後転 20
    a[0] = 20 + id;
    ToMD.write(a, 1);
    a[0] = abs(mySpeed);
    ToMD.write(a, 1);
  } else if (mySpeed == 0) {
    //ストップ 30
    a[0] = 30 + id;
    ToMD.write(a, 1);
  }
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int F_IR_wrap_around(int IR) {
  // IR -= 1;
  // const int C_IR_length = 16.25;
  // //距離が500時、16.25°。距離がxの時、arcsin(140 / x)°
  int IR_wrap_degree;

  // if (IR_distance == 0) {
  // IR_wrap_degree = 0;
  // } else if (IR_distance == 1 || IR_distance == 2) {
  IR_wrap_degree = F_IR_mawarikomi(IR, 65);
  // } else if (IR_distance == 3) {
  // IR_wrap_degree = F_IR_mawarikomi_mae(IR, 5);
  // }
  return IR_wrap_degree;
}

int F_IR_mawarikomi(int IR, int leng) {
  const int C_IR_length = leng; // 300mm

  int result;
  //回り込み
  if (IR == 0) {
    result = 0;
  } else if ((20 >= IR || IR >= 340)) {
    result = 360;
  } else if ((25 <= IR && IR <= 25)) {
    result = 5;
  } else if ((20 <= IR && IR <= 70)) {
    result = IR + 70;

  } else if (70 <= IR && IR <= 180) {
    result = IR + C_IR_length;

  } else if (180 <= IR && IR <= 290) {
    result = IR - C_IR_length;

  } else if ((290 <= IR && IR <= 340)) {
    result = IR - 70;

  } else if (335 <= IR && IR <= 335) {
    result = 355;
  }
  return result;
}

int F_IR_mawarikomi_mae(int IR, int leng) {
  const int C_IR_length = leng; // 300mm

  int result;
  //回り込み
  if (IR == 0) {
    result = 0;
  } else if ((30 >= IR || IR >= 330)) {
    result = 360;
  } else if ((30 <= IR && IR <= 45)) {
    result = 65;
  } else if ((45 <= IR && IR <= 70)) {
    result = IR + 70;

  } else if (70 <= IR && IR <= 180) {
    result = IR + C_IR_length;

  } else if (180 <= IR && IR <= 290) {
    result = IR - C_IR_length;

  } else if ((290 <= IR && IR <= 315)) {
    result = IR - 70;

  } else if (315 <= IR && IR <= 330) {
    result = 295;
  }
  return result;
}
