#include <Wire.h>
#include <MPU6050_6Axis_MotionApps20.h>

#define Gyro_X 11
#define Gyro_Y -15
#define Gyro_Z 24
#define Accel_Z 1253



MPU6050 mpu;
static uint8_t mpuIntStatus;
static bool dmpReady = false;  // set true if DMP init was successful
static uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)

int16_t  Gyro_Now = 0, Gyro = 0, Gyro_Offset = 0;
uint16_t fifoCount;
uint8_t fifoBuffer[64]; // FIFO storage buffer                 // orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector



void setup() {

  Serial.begin(115200);
  Serial.println("Hello_BASE");
  //-------------------------------------------------------
  F_ESP_setup();
  Wire.begin();
  Gryo_Start();
  F_MD_setup();
  Serial.println("FinishSetup");

}

int tilt = 0;
void loop() {
  //  mySerial.write(10);
  //  mySerial.write(150);
  //  delay(3000);
  //  mySerial.write(40);
  //  mySerial.write(int(0));


  //----------------------------------------------
  F_ESP_read();
  Serial.print(" an");
  Serial.print( F_ESP_angle());
  Serial.print(" sp");
  Serial.print( F_ESP_speed());
  Serial.print(" sw");
  Serial.print( F_ESP_switch());
  Serial.print(" gy");
  
  int Gyro_value = GyroGet();

  if (F_ESP_switch() == 4) {
    tilt = 45;
  }
  if (F_ESP_switch() == 1) {
    tilt = 0;
  }
  if (F_ESP_switch() == 2) {
    tilt = -45;
  }

  int GYRO_DEGDEG = F_360_correct(Gyro_value - tilt);
  int GO_DEGDEG;

  if (F_ESP_angle() != 0) {
    GO_DEGDEG = F_360_correct(F_ESP_angle()  - Gyro_value);
  } else {
    GO_DEGDEG = 0;
  }

  F_MD_rotate2(GO_DEGDEG, GYRO_DEGDEG, map(F_ESP_speed(), 0, 100, 40, 254));

}


int F_360_correct(int degree){
  while(degree < 0){
    degree += 360;
  }
  while(degree > 360){
    degree -= 360;
  }
  return degree;
}




//#include <Wire.h>
//#include <MPU6050_6Axis_MotionApps20.h>
//
//#define Gyro_X 11
//#define Gyro_Y -15
//#define Gyro_Z 24
//#define Accel_Z 1253
//
//MPU6050 mpu;
//static uint8_t mpuIntStatus;
//static bool dmpReady = false;  // set true if DMP init was successful
//static uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
//
//int16_t  Gyro_Now = 0, Gyro = 0, Gyro_Offset = 0;
//uint16_t fifoCount;
//uint8_t fifoBuffer[64]; // FIFO storage buffer                 // orientation/motion vars
//Quaternion q;           // [w, x, y, z]         quaternion container
//VectorFloat gravity;    // [x, y, z]            gravity vector
//float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
//
//void setup() {
//  Serial.begin(115200);
//  Wire.begin();
//
//  Gryo_Start();
//}
//
//void loop() {
//GyroGet();
//
//Serial.println(Gyro);
//}
//
//void GyroGet() {
//  mpuIntStatus = false;
//  mpuIntStatus = mpu.getIntStatus();
//  fifoCount = mpu.getFIFOCount();
//  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
//    mpu.resetFIFO();
//  }
//  else if (mpuIntStatus & 0x02) {
//    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
//    mpu.getFIFOBytes(fifoBuffer, packetSize);
//    fifoCount -= packetSize;
//    mpu.dmpGetQuaternion(&q, fifoBuffer);
//    mpu.dmpGetGravity(&gravity, &q);
//    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
//    Gyro_Now = degrees(ypr[0]) + 180;
//    Gyro = Gyro_Now + Gyro_Offset;
//    if (Gyro < 0) Gyro += 360;
//    if (Gyro > 359) Gyro -= 360;
//  }
//}
//
//void Gryo_Start() {
//  mpu.initialize();
//  if (mpu.testConnection() != true) {
//    Serial.println("MPU disconection");
//    while (true) {}
//  }
//  if (mpu.dmpInitialize() != 0) {
//    Serial.println("MPU break");
//    while (true) {}
//  }
//  mpu.setXGyroOffset(Gyro_X);
//  mpu.setYGyroOffset(Gyro_Y);
//  mpu.setZGyroOffset(Gyro_Z);
//  mpu.setZAccelOffset(Accel_Z);
//  mpu.setDMPEnabled(true);
//  mpuIntStatus = mpu.getIntStatus();
//  dmpReady = true;
//  packetSize = mpu.dmpGetFIFOPacketSize();
//}
