
//モーターマイコン
#include "mbed.h"

static BufferedSerial ToMain(PA_9, PB_7); // TXはダミー

static Timer ti;

static PwmOut MT_A[4] = {PA_3, PA_10, PB_0, PB_4};
static PwmOut MT_B[4] = {PA_1, PA_11, PA_4, PB_6};

int main(void) {
  ToMain.set_baud(57600);
  ToMain.set_format(
      /* bits */ 8, /* parity */ BufferedSerial::None, /* stop bit */ 1);

  ti.start();

  char buf[32] = {0};

  // int mytime;

  for (int i = 0; i < 4; i++) {
    MT_A[i].period(0.001f);
    MT_B[i].period(0.001f);
    MT_A[i] = 0;
    MT_B[i] = 0;
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

    if (myspeed > 0.5) {
      myspeed = 0.5;
    }

    if (rotation == 0) {
      MT_A[id] = 0;
      MT_B[id] = 0;
    } else if (rotation == 1) {
      MT_A[id] = myspeed;
      MT_B[id] = 0;
    } else if (rotation == 2) {
      MT_A[id] = 0;
      MT_B[id] = myspeed;
    } else if (rotation == 3) {
      MT_A[id] = 1;
      MT_B[id] = 1;
    }
  }
}

//---------------------------------------------
// #include "mbed.h"

// static DigitalOut L0(PB_3);
// static DigitalOut L1(PA_8);
// static DigitalOut L2(PA_11);
// static DigitalOut L3(PA_12);

// int main(){
//     L1 = 1;
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

// // static DigitalOut IN_A(PF_0);
// // static DigitalOut IN_B(PF_1);
// // static DigitalOut IN_C(PB_0);
// static DigitalOut  MUXs[]  = {  //  配列を用意します
//     DigitalOut( PF_0),  //  配列の1番目の要素をLED4で初期化したDigitalOutに
//     DigitalOut( PF_1 ),  //  ..
//     DigitalOut( PB_0 ),  //  ..
// };

// static DigitalOut inh(PB_1);

// static AnalogIn OUT_X1(PA_0);
// static AnalogIn OUT_Y1(PA_1);
// static AnalogIn OUT_X2(PA_6);
// static AnalogIn OUT_Y2(PA_7);

// static DigitalOut  leds[]  = {  //  配列を用意します
//     DigitalOut( PB_3 ),  //  配列の1番目の要素をLED4で初期化したDigitalOutに
//     DigitalOut( PA_8 ),  //  ..
//     DigitalOut( PA_11 ),  //  ..
//     // DigitalOut( PA_12 )   //  ..
// };

// static BufferedSerial serial(PA_2, PA_15);

// int main(){
//     printf("Start\r\n");
//     inh = 0;
//     HAL_Delay(100);

//     uint16_t analog_value[32];

//     while(true){

//         for( int j = 0; j < 8; j++ ){
//             for ( int i = 0; i < 3; i++ ) {
//                 leds[ i ]   = (j >> i) & 0x1;  //  変数 count
//                 の値をLED4〜LED1に表示(二進値) MUXs[ i ]   = (j >> i) & 0x1;
//             }

//         analog_value[j] = OUT_X1.read_u16();
//         analog_value[j + 8] = OUT_Y1.read_u16();
//         analog_value[j + 16] = OUT_X2.read_u16();
//         analog_value[j + 24] = OUT_Y2.read_u16();
//         }

//         for(int n = 0; n < 32; n++){
//             printf("%d ",analog_value[n]%1000);
//             if(n == 7||n == 15||n == 23||n == 31){
//                 printf(" - ");
//             }
//         }
//         printf("\r\n");

//         wait_us(100000);
//     }
// }

//--------------------------------------------
// #include "mbed.h"

// static PwmOut MT_A[4]  = { PA_9, PA_10,    PB_4, PA_1 };
// static PwmOut MT_B[4]  = { PA_4, PA_11,    PB_6, PA_3 };

// int main(){
// 	for(int i = 0; i < 4; i++ ){
// 		MT_A[i].period(0.001f);
// 		MT_B[i].period(0.001f);

// 		MT_A[i] = 0;
// 		MT_B[i] = 0;
// 	}
// 	HAL_Delay(3000);

// 	while(true)  {

// 		int aa = 2;
// 		int bb = 3;
// 		float speed = 0.4;

// 		printf("aa\r\n");
// 		MT_A[aa] = speed;
// 		MT_B[aa] = 0;
// 		MT_A[bb] = speed;
// 		MT_B[bb] = 0;
// 		HAL_Delay(3000);

// 		printf("bb\r\n");
// 		MT_A[aa] = 1;
// 		MT_B[aa] = 1;
// 		MT_A[bb] = 1;
// 		MT_B[bb] = 1;
// 		HAL_Delay(100);

// 		printf("cc\r\n");
// 		MT_A[aa] = 0;
// 		MT_B[aa] = speed;
// 		MT_A[bb] = 0;
// 		MT_B[bb] = speed;
// 		HAL_Delay(3000);

// 		printf("dd\r\n");
// 		MT_A[aa] = 1;
// 		MT_B[aa] = 1;
// 		MT_A[bb] = 1;
// 		MT_B[bb] = 1;
// 		HAL_Delay(100);
// 	}

// }

// //--------------------------------------------
// #include "mbed.h"

// static PwmOut MT_0A = PA_3;
// static PwmOut MT_0B = PA_1;
// static PwmOut MT_1A = PA_10;
// static PwmOut MT_1B = PA_11;
// static PwmOut MT_2A = PB_0;
// static PwmOut MT_2B = PA_4;
// static PwmOut MT_3A = PB_4;//
// static PwmOut MT_3B = PB_6;//

// int main(){
//     float fla = 0.001;
//     MT_0A.period(fla);
//     MT_0B.period(fla);
//     MT_1A.period(fla);
//     MT_1B.period(fla);
//     MT_2A.period(fla);
//     MT_2B.period(fla);
//     MT_3A.period(fla);
//     MT_3B.period(fla);

//     HAL_Delay(3000);

//     float speed = 0.3;
//     while(true){
//         MT_0A = speed;
//         MT_0B = 0;
//         MT_1A = speed;
//         MT_1B = 0;
//         MT_2A = 0;
//         MT_2B = speed;
//         MT_3A = 0;
//         MT_3B = speed;
//         HAL_Delay(1000);

//         MT_0A = 1;
//         MT_0B = 1;
//         MT_1A = 1;
//         MT_1B = 1;
//         MT_2A = 1;
//         MT_2B = 1;
//         MT_3A = 1;
//         MT_3B = 1;
//         HAL_Delay(1000);

//         MT_0A = 0;
//         MT_0B = speed;
//         MT_1A = 0;
//         MT_1B = speed;
//         MT_2A = speed;
//         MT_2B = 0;
//         MT_3A = speed;
//         MT_3B = 0;
//         HAL_Delay(1000);

//         MT_0A = 1;
//         MT_0B = 1;
//         MT_1A = 1;
//         MT_1B = 1;
//         MT_2A = 1;
//         MT_2B = 1;
//         MT_3A = 1;
//         MT_3B = 1;
//         HAL_Delay(1000);

//     }

// }

//--------------------------------------------
// #include "mbed.h"

// #define NUM_OF_LEDS 8
// #define LEVEL1 0.5
// #define LEVEL2 0.8

// PwmOut leds[NUM_OF_LEDS] = {PB_0, PA_10,     PB_6,    PA_11, PB_4, PA_1,
// PA_3, PA_4};

// void initPwm();

// int main() {
//   initPwm();
//   while (1) {
//     for (int i = 0; i < NUM_OF_LEDS; i++) {
//       for (int j = 0; j < NUM_OF_LEDS; j++) {
//         leds[j] = 0.01;
//         if (j == i) {
//           leds[i] = 0.9;
//         }
//       }
//       HAL_Delay(300);
//     }
//   }
// }

// void initPwm() {
//   for (int i = 0; i < NUM_OF_LEDS; i++) {
//     leds[i].period_ms(1);
//   }
// }

//--------------------------------------------
// #include "mbed.h"

// static DigitalOut L0(PB_1);
// static DigitalOut L1(PB_3);

// int main(){

//     while(true){
//         L0 = 0;
//         L1 = 0;
//         HAL_Delay(2000);
//         L0 = 1;
//         L1 = 1;
//         HAL_Delay(2000);
//     }
// }