#include "ball_sensor.h"

_IR::_IR(PinName ch0, PinName ch1, PinName ch2, PinName an0, PinName an1) {
  MUXs[0] = ch0;
  MUXs[1] = ch1;
  MUXs[2] = ch2;
  OUT_X = an0;
  OUT_Y = an1;
}

int _IR::SensorRead(int SensorNum) {
  MUXs = SensorNum;
  return OUT_X.read
}

// #include <mbed.h>

// class IR {
// public:
//   IR(PinName pin);
//   int read();

// private:
//   AnalogIn pin;
//   IR(int p) { pin = p; }
// };

// IR::IR(PinName pin) : _pin(pin) {}

// int read() { return _pin.read(); }

// int main() {
//   IR ir(PC_0);
//   while (1) {
//     printf("%d\r", ir.read());

//     void readPins() {
//       static DigitalOut MUXs[] = {
//           DigitalOut(PC_2),
//           DigitalOut(PC_3),
//           DigitalOut(PC_4),
//       };

//       BusOut MUXss(PC_2, PC_3, PC_4);

//       static AnalogIn OUT_X(PC_0);
//       static AnalogIn OUT_Y(PC_1);

//       int analog_value[16] = {0};

//       Timer timer;
//       uint16_t delay;
//       timer.start();
//       timer.reset();
//       for (uint8_t i = 0; i < 8; i++) {
//         MUXss = i;
//         // wait_us(10000); //正常
//         wait_us(100); //だめ
//         analog_value[i] = (uint16_t)(OUT_X.read() * 100);
//       }
//       delay = timer.read_us();
//     }