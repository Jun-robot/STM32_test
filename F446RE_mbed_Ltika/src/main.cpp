// #include "mbed.h"

// static DigitalOut led(PC_13);
// static DigitalOut led2(PC_14);

// // static BufferedSerial serial(D8, D2);
// static BufferedSerial ToMD(PC_12, PD_2);

// static DigitalIn toggle(PA_12);

// int main(void) {
//   toggle.mode(PullDown);
//   ToMD.set_baud(57600);
//   ToMD.set_format(
//       /* bits */ 8, /* parity */ BufferedSerial::None, /* stop bit */ 1);

//   led = 0;
//   led2 = 1;

//   while (true) {
//     char a[1] = {0};
//     if (toggle == 0) {
//       led = !led;
//       led2 = !led2;
//       a[0] = 10;
//       ToMD.write(a, 1);
//       a[0] = 155;
//       ToMD.write(a, 1);
//       HAL_Delay(3000);

//       led = !led;
//       led2 = !led2;
//       a[0] = 40;
//       ToMD.write(a, 1);
//       a[0] = 00;
//       ToMD.write(a, 1);
//       HAL_Delay(300);
//     }
//   }
// }

// //-----------------------------------------
// #define mC 261.626
// #define mD 293.665
// #define mE 329.628
// #define mF 349.228
// #define mG 391.995
// #define mA 440.000
// #define mB 493.883

// #include "mbed.h"

// PwmOut sp1(PC_7);

// int main() {
//     float mm[]={mC,mD,mE,mF,mG,mA,mB,mC*2};
//     int i;

//     for (i=0;i<sizeof(mm);i++) {
//         sp1.period(1.0/mm[i]);
//         sp1.write(0.5f);
//         wait_us(0.5f * 1000000);
//         sp1.write(0.0f);
//     }
//     while (1);
// }

//---------------------------------------------
// #include "mbed.h"

// static DigitalOut MUXs[] = {
//     DigitalOut(PC_2), //  配列の1番目の要素をLED4で初期化したDigitalOutに
//     DigitalOut(PC_3), //  ..
//     DigitalOut(PC_4), //  ..
// };

// static AnalogIn OUT_X(PC_0);
// static AnalogIn OUT_Y(PC_1);

// static BufferedSerial serial(PA_2, PA_3);

// int main() {
//   printf("Start\r\n");
//   HAL_Delay(100);
//   int degree;
//   int analog_value[16];
//   int duration[16];
//   int pre_duration[16];

//   const float sine[] = {
//       0, 0.38268,  0.70711,  0.92388,  1,  0.92388,  0.70711,  0.38268,
//       0, -0.38268, -0.70711, -0.92388, -1, -0.92388, -0.70711, -0.38268};
//   const float cosine[] = {
//       1,  0.92388,  0.70711,  0.38268,  0, -0.38268, -0.70711, -0.92388,
//       -1, -0.92388, -0.70711, -0.38268, 0, 0.38268,  0.70711,  0.92388};

//   while (true) {

//     int best_id = 0;
//     int best_duration = 0;

//     for (int j = 0; j < 8; j++) {
//       for (int i = 0; i < 3; i++) {
//         MUXs[i] = (j >> i) & 0x1;
//       }
//       analog_value[j] = 60 - (OUT_X.read_u16() / 1000);
//       analog_value[j + 8] = 60 - (OUT_Y.read_u16() / 1000);
//     }

//     for (int id = 0; id < 16; id++) {
//       duration[id] = analog_value[id] * 0.6 + pre_duration[id] * 0.4;
//       // if (id == 0) {
//       //   duration[0] *= 1.12;
//       // }
//       pre_duration[id] = duration[id];

//       best_duration = max(best_duration, duration[id]);
//       if (best_duration == duration[id]) {
//         best_id = id;
//       }

//       // printf("%d ",duration[id]);
//     }
//     duration[7] = (duration[6] + duration[8]) / 2;
//     duration[15] = (duration[0] + duration[14]) / 2;

//     for (int id = 0; id < 16; id++) {
//       printf("%d ", duration[id]);
//     }

//     if (best_duration > 0) {
//       //ボールがあったとき
//       int read_id[5];
//       int x_axis = 0;
//       int y_axis = 0;
//       read_id[0] = best_id - 2;
//       read_id[1] = best_id - 1;
//       read_id[2] = best_id;
//       read_id[3] = best_id + 1;
//       read_id[4] = best_id + 2;
//       for (int i = 0; i < 5; i++) {
//         if (read_id[i] > 15) {
//           read_id[i] -= 16;
//         }
//         if (read_id[i] < 0) {
//           read_id[i] += 16;
//         }

//         x_axis += duration[read_id[i]] * cosine[read_id[i]];
//         y_axis += duration[read_id[i]] * sine[read_id[i]];
//       }

//       degree = (atan2((y_axis), (x_axis)) * 180 / 3.1415);
//       if (degree > 360) {
//         degree -= 360;
//       } else if (degree < 1) {
//         degree += 360;
//       }

//     } else {
//       //ボールがなかった時
//       degree = 0;
//     }

//     printf("  %d , %d \r\n", degree, best_id);
//   }
// }

//---------------------------------------------
// #include "mbed.h"

// static DigitalOut L0(PB_3);
// static DigitalOut L1(PA_8);
// static DigitalOut L2(PA_11);
// static DigitalOut L3(PC_12);

// int main(){
//     printf(" START \r\n");
//     while(true){
//         L0 = !L0;
//         L1 = !L1;
//         L2 = !L2;
//         L3 = !L3;
//         HAL_Delay(100);
//     }
// }

//---------------------------------------------
// #include "mbed.h"
// #include <mbed.h>

// static DigitalOut MUXs[] = {
//     DigitalOut(PC_2),
//     DigitalOut(PC_3),
//     DigitalOut(PC_4),
// };

// BusOut MUXss(PC_2, PC_3, PC_4);

// static AnalogIn OUT_X(PC_0);
// static AnalogIn OUT_Y(PC_1);

// static BufferedSerial serial(PA_2, PA_3);
// int analog_value[16] = {0};

// Timer timer;
// uint16_t delay;

// void readPins() {
//   timer.start();
//   timer.reset();
//   for (uint8_t i = 0; i < 8; i++) {
//     MUXss = i;
//     // wait_us(10000); //正常
//     wait_us(100); //だめ
//     analog_value[i] = (uint16_t)(OUT_X.read() * 100);
//   }
//   delay = timer.read_us();
// }

// int main() {
//   while (1) {
//     readPins();
//     printf("Time:%d\t", delay);
//     for (uint8_t i = 0; i < 8; i++) {
//       printf("%d\t", analog_value[i]);
//     }
//     printf("\r\n");
//   }
// }

//---------------------------------------------
// #include "mbed.h"

// static DigitalOut MUXs[] = {
//     DigitalOut(PC_2),
//     DigitalOut(PC_3),
//     DigitalOut(PC_4),
// };

// // BusOut MUXss(PC_2, PC_3, PC_4);

// static AnalogIn OUT_X(PC_0);
// static AnalogIn OUT_Y(PC_1);

// static BufferedSerial serial(PA_2, PA_3);
// int analog_value[16] = {0};
// Timer timer;
// uint16_t delay;

// int main() {
//   printf("Start\r\n");
//   HAL_Delay(100);

//   MUXs[0] = 1;
//   MUXs[1] = 1;
//   MUXs[2] = 1;

//   uint16_t num = 500;

//   while (true) {
//     MUXs[0] = 1;
//     MUXs[1] = 1;
//     MUXs[2] = 1;
//     HAL_Delay(1000);

//     timer.start();
//     timer.reset();
//     MUXs[0] = 1;
//     MUXs[1] = 1;
//     MUXs[2] = 0;

//     uint16_t time[num] = {0};
//     uint16_t value[num] = {0};

//     uint16_t n = 0;
//     while (n < num) {
//       n++;
//       value[n] = (uint16_t)(OUT_X.read() * 1000);
//       time[n] = timer.read_us();
//     }

//     for (int i = 0; i < num; i++) {
//       printf("Time:%d\t", time[i]);
//       printf("%d\t", value[i]);
//       printf("\r\n");
//     }
//   }
// }

//--------------------------------------------
// #include "mbed.h"

// static PwmOut MT_0A = PA_10;
// static PwmOut MT_0B = PA_11;
// static PwmOut MT_1A = PC_8;
// static PwmOut MT_1B = PC_9;

// // static PwmOut MT_0A = PA_11;
// // static PwmOut MT_0B = PA_10;
// // static PwmOut MT_1A = PC_9;
// // static PwmOut MT_1B = PC_8;

// int main() {
//   float fla = 0.001;
//   MT_0A.period(fla);
//   MT_0B.period(fla);
//   MT_1A.period(fla);
//   MT_1B.period(fla);

//   HAL_Delay(3000);

//   float speed = 0.9;
//   // HAL_Delay(2000);
//   while (true) {
//     MT_0A = speed;
//     MT_0B = 0;
//     MT_1A = speed;
//     MT_1B = 0;
//     HAL_Delay(500);

//     MT_0A = 0;
//     MT_0B = 0;
//     MT_1A = 0;
//     MT_1B = 0;
//     HAL_Delay(2000);

//     MT_0A = 0;
//     MT_0B = speed;
//     MT_1A = 0;
//     MT_1B = speed;
//     HAL_Delay(500);

//     MT_0A = 0;
//     MT_0B = 0;
//     MT_1A = 0;
//     MT_1B = 0;
//     HAL_Delay(2000);
//   }
// }

// --------------------------------------------

//モーターマイコン
#include "mbed.h"

static BufferedSerial ToMain(PB_6, PB_7); // TXはダミー

static Timer ti;

//右
static PwmOut MT_A[2] = {PC_8, PA_10};
static PwmOut MT_B[2] = {PC_9, PA_11};

static DigitalOut MT_EN[4] = {PC_12, PC_13, PC_14, PC_15};

int main(void) {
  ToMain.set_baud(57600);
  ToMain.set_format(
      /* bits */ 8, /* parity */ BufferedSerial::None, /* stop bit */ 1);
  ti.start();

  char buf[32] = {0};

  for (int i = 0; i < 2; i++) {
    MT_A[i].period(0.001f);
    MT_B[i].period(0.001f);
    MT_A[i] = 0;
    MT_B[i] = 0;
  }

  for (int i = 0; i < 4; i++) {
    MT_EN[i] = 1;
  }

  int id = 0;
  int rotation = 0;
  //   int pre_rotation = 1;
  float myspeed = 0.5;

  while (true) {
    for (int ii = 0; ii < 2; ii++) {
      if (ToMain.readable() == 1) {
        if (uint32_t num = ToMain.read(buf, sizeof(buf))) {
          if (buf[0] < 35) {
            id = buf[0] % 10;
            rotation = buf[0] / 10;
            // pre_rotation = rotation;
          } else {
            myspeed = buf[0];          // 35-255
            myspeed = myspeed * 0.002; // 0-0.5
          }
        }
      }
    }

    // printf("id:%d\t rot:%d\t,spe:%d\t", id, rotation, int(myspeed * 100));

    if (myspeed > 0.5) {
      myspeed = 0.5;
    }

    int hoge = true; // true のとき右
    if (hoge == true) {
      if (id == 0 || id == 1) {
        if (rotation == 0) {
          MT_A[id] = 0;
          MT_B[id] = 0;

          MT_EN[0] = 1;
          MT_EN[1] = 0;
          MT_EN[2] = 0;
          MT_EN[3] = 0;

        } else if (rotation == 1) {
          MT_A[id] = myspeed;
          MT_B[id] = 0;

          MT_EN[0] = 0;
          MT_EN[1] = 1;
          MT_EN[2] = 0;
          MT_EN[3] = 0;
        } else if (rotation == 2) {
          MT_A[id] = 0;
          MT_B[id] = myspeed;

          MT_EN[0] = 0;
          MT_EN[1] = 0;
          MT_EN[2] = 0;
          MT_EN[3] = 1;
        } else if (rotation == 3) {
          MT_A[id] = 1;
          MT_B[id] = 1;

          MT_EN[0] = 0;
          MT_EN[1] = 1;
          MT_EN[2] = 1;
          MT_EN[3] = 1;
        }
      } else {
        // 34は無視
      }
    } else {
      if (id == 0 || id == 1) {
        //無視
      } else if (id == 2 || id == 3) {
        id = id - 2;

        if (rotation == 0) {
          MT_A[id] = 0;
          MT_B[id] = 0;

          MT_EN[0] = 1;
          MT_EN[1] = 0;
          MT_EN[2] = 0;
          MT_EN[3] = 0;

        } else if (rotation == 1) {
          MT_A[id] = myspeed;
          MT_B[id] = 0;

          MT_EN[0] = 0;
          MT_EN[1] = 1;
          MT_EN[2] = 0;
          MT_EN[3] = 0;
        } else if (rotation == 2) {
          MT_A[id] = 0;
          MT_B[id] = myspeed;

          MT_EN[0] = 0;
          MT_EN[1] = 0;
          MT_EN[2] = 0;
          MT_EN[3] = 1;
        } else if (rotation == 3) {
          MT_A[id] = 1;
          MT_B[id] = 1;

          MT_EN[0] = 0;
          MT_EN[1] = 1;
          MT_EN[2] = 1;
          MT_EN[3] = 1;
        }
      }
    }
    // printf("\n");
  }
}
