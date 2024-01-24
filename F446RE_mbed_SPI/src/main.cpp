#include "mbed.h"

SPI spi(PA_7,PA_6,PA_5); // mosi, miso, sclk
DigitalOut cs(PB_6);
DigitalOut cs2(PC_7);

int main(void)
{

    int mytime;
    spi.format(8, 3);
    spi.frequency(1000000);
    int num = 0;

    while (1) {  
        printf("SEND_%d  ", (num+10)%256);
        cs = 1;
        cs = 0;
        int receive1 = spi.write(num);
        printf("RECEIVE1_%d", receive1);
        cs = 1;


        cs2 = 1;
        cs2 = 0;
        int receive2 = spi.write(num);
        printf("  RECEIVE2_%d", receive2);
        cs2 = 1;

        printf("  DIFF_%d\n", receive2-receive1);
        
        num++;
        // HAL_Delay(10);
    }
}