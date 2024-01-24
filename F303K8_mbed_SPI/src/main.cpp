#include "mbed.h"

SPISlave device(PA_7,PA_6,PA_5, PA_4);
// SPISlave device(PB_5,PB_4,PA_5, PA_4);

int main() {
    device.format(8, 3);
    device.frequency(1000000);

    while(1) {
        if(device.receive()) {
            int v = device.read(); 
            v = (v + 10) % 0x100;
            device.reply(v);
        }
    }
}