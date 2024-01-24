#include <mbed.h>

#include "ball_sensor.h"

int main() {

  static BufferedSerial serial(PA_2, PA_3);
  _IR ir(PC_2, PC_3, PC_4, PC_0, PC_1);

  while (1) {
    ir.SensorRead(10);
  }
}