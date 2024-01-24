 #include "BluetoothSerial.h"

BluetoothSerial SerialBT;
int controller_angle;
int controller_speed;
int controller_switch;


void wireless_setup() {
  SerialBT.begin("ESP32_BASE");
}

int F_ESP_angle() {
  return controller_angle;
}

int F_ESP_speed() {
  return controller_speed;
}

int F_ESP_switch() {
  return controller_switch;
}

int pre_controller_switch = 0;
void wireless_read() {
  if (SerialBT.available()) {
    String str = SerialBT.readStringUntil('#');
    long value = str.toInt();

    if ((value / 10000) == 0) {
      controller_angle = 0;
    } else {
      controller_angle = 450 - (value / 10000);
    }

    if (controller_angle > 360) {
      controller_angle -= 360;
    }

    controller_speed = value % 10000 / 10;
    controller_switch = value % 10;
  } else {
  }

  //Switch補正
  if (pre_controller_switch != 0) {
    controller_switch = 0;
  }
  pre_controller_switch = controller_switch;
}
