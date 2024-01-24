#include "mbed.h"

static DigitalOut led(D12);
static DigitalOut led1(LED1);

static BufferedSerial serial(D8, D2);
// static BufferedSerial serial(USBTX, USBRX);
// static BufferedSerial serial(PA_2, PA_3);

// I2C i2c(I2C_SDA, I2C_SCL);
// I2C i2c(PB_9, PB_8);


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
        if(mytime + 100 <= ti.read_ms()){
            mytime = ti.read_ms() + 100;
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