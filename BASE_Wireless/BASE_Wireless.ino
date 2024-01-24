void setup() {
  Serial.begin(115200);
  F_MD_setup();
  wireless_setup();
  gyro_setup();
  Serial.println("FinishSetup");
}

int tilt = 0;
void loop() {
  wireless_read();
  //  Serial.print(" an");
  //  Serial.print( F_ESP_angle());
  //  Serial.print(" sp");
  //  Serial.print( F_ESP_speed());
  //  Serial.print(" sw");
  //  Serial.print( F_ESP_switch());
  //  Serial.print(" gy");
  //  Serial.print( gyro_get());
  //  Serial.println();

  int Gyro_value = gyro_get();

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

  F_MD_rotate2(GO_DEGDEG, GYRO_DEGDEG, map(F_ESP_speed(), 0, 100, 40, 200));

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
