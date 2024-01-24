#ifndef _H_BALL_SENSOR_ // include guard このサイトわかりやすいよ↓
#define _H_BALL_SENSOR_ // https://captain-cocco.com/c-include-guard/

#include <mbed.h>

class _IR {
public:
  //ピン定義
  BusOut MUXs[3];
  AnalogIn OUT_X;
  AnalogIn OUT_Y;
  _IR(PinName ch0, PinName ch1, PinName ch2, PinName an0, PinName an1);

  int SensorRead(int SensorNum);

private:
}

#endif
//_H_BALL_SENSOR_