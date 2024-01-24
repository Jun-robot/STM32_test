#include "mbed.h"

static DigitalOut led(PA_8);
static AnalogIn ball(PA_7);

// static BufferedSerial serial(USBTX, USBRX);//USBTX=PA_2 USBRX=PA_15

int main(void)
{

   printf("start");
   while(true){
        led = !led;
        wait_us(1000);
        int a = ball.read_u16();
        // a = 64522;
        printf("%d\n", a);
        
        // printf("\r\n-- Start: SerialBridge\r\n");

        // char buf[5];
        // snprintf(buf,5,"%d",a);
        // serial.write(buf,5);
   }
}