#include <SoftwareSerial.h>

SoftwareSerial MT_R_Serial(2, 3);//rx,tx
SoftwareSerial MT_L_Serial(4, 5);//rx,tx

const int MOTOR_DEG[] = {45, 135, 225, 315}; // モーター配置角度
int out_speed[4];//出力値




//---------------------------------------


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
      sine_power[i] = (sin(sine_thita * PI / 180));
      
      //ジャイロの値を追加する。
      sine_power[i] = sine_power[i] + ((gyro_speed / 255*abs(sine_power[i])) * 0.5);
      //ここまで
      
      big_power = max(big_power, abs(sine_power[i]));
    }
    float  aaa = 1 / big_power;
    for (int i = 0; i < 4; i++) {
      lineIr_speed[i] = ((sine_power[i] * aaa)) * SPEED;
    }
  }else if(gyro_speed != 0){
    for (int i = 0; i < 4; i++) {
      lineIr_speed[i] = gyro_speed;
    }
  }else{
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
      out_speed[i] = constrain(out_speed[i], 40, 254);
    } else if (out_speed[i] < 0) {
      out_speed[i] = constrain(out_speed[i], -254, -40);
    }
    F_speed_send(i, out_speed[i]);
  }
}











void F_MD_debug() {
  Serial.print("  0:");
  Serial.print(out_speed[0]);
  Serial.print(" 1:");
  Serial.print(out_speed[1]);
  Serial.print(" 2:");
  Serial.print(out_speed[2]);
  Serial.print(" 3:");
  Serial.print(out_speed[3]);
}

void F_MD_rotate(int LINEIR, int GYRO, int SPEED) {
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
      sine_power[i] = (sin(sine_thita * PI / 180));
      big_power = max(big_power, abs(sine_power[i]));
    }
    float  aaa = 1 / big_power;
    for (int i = 0; i < 4; i++) {
      lineIr_speed[i] = ((sine_power[i] * aaa)) * SPEED;
    }
  }else{
    for (int i = 0; i < 4; i++) {
      lineIr_speed[i] = 0;
    }
  }


  for (int i = 0; i < 4; i++) {
    out_speed[i] = gyro_speed * 0.6 + lineIr_speed[i] * 1;
  }

  for (int i = 0; i < 4; i++) {
    if (out_speed[i] == 0) {
      out_speed[i] = 0;
    } else if (out_speed[i] > 0) {
      out_speed[i] = constrain(out_speed[i], 40, 254);
    } else if (out_speed[i] < 0) {
      out_speed[i] = constrain(out_speed[i], -254, -40);
    }
    F_speed_send(i, out_speed[i]);
  }
}

void F_MD_setup() {
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, INPUT);
  pinMode(5, OUTPUT);


  MT_R_Serial.begin(57600);
  MT_L_Serial.begin(57600);
}


void F_speed_send(int id , int mySpeed) {
  if (id == 0 || id == 1) {
    if (mySpeed == -1) {
      //ブレーキ 30
      MT_R_Serial.write(30 + id);
    } else if (mySpeed > 0) {
      //正転 10
      MT_R_Serial.write(10 + id);
      MT_R_Serial.write(mySpeed);
    } else if (mySpeed < 0) {
      //後転 20
      MT_R_Serial.write(20 + id);
      MT_R_Serial.write(abs(mySpeed));
    } else if (mySpeed == 0) {
      //ストップ 0
      MT_R_Serial.write(0 + id);
    }
  } else if (id == 2 || id == 3) {
    id = id - 2;
    if (mySpeed == -1) {
      //ブレーキ 30
      MT_L_Serial.write(30 + id);
    } else if (mySpeed > 0) {
      //正転 10
      MT_L_Serial.write(10 + id);
      MT_L_Serial.write(mySpeed);
    } else if (mySpeed < 0) {
      //後転 20
      MT_L_Serial.write(20 + id);
      MT_L_Serial.write(abs(mySpeed));
    } else if (mySpeed == 0) {
      //ストップ 0
      MT_L_Serial.write(0 + id);
    }
  }
}



//-----------------------------------------------





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
