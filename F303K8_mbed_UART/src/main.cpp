#include "mbed.h"

static DigitalOut led(D12);
static DigitalOut led1(LED1);


static BufferedSerial serial(D1,D0);

static Timer ti;


int main(void)
{
    serial.set_baud(57600);
    serial.set_format(
        /* bits */ 8,/* parity */ BufferedSerial::None,/* stop bit */ 1
    );

    ti.start();

    char a[1] = {0};
    char buf[32] = {0};

    int mytime;

    while (1) {        
        if(mytime + 500 <= ti.read_ms()){
            mytime = ti.read_ms() + 500;
        }else if(mytime <= ti.read_ms()){
            a[0] = {'1'};
            serial.write(a, 1);
            led1 = 1;
        }else{
            a[0] = {'0'};
            serial.write(a, 1);
            led1 = 0;
        }

        if(serial.readable() == 1){
            if (uint32_t num = serial.read(buf, sizeof(buf))) {
                if(buf[0] == '1'){
                    led = 1;
                }else if(buf[0]=='0'){
                    led=0;
                }
            }
        }
    }
}