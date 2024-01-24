#include "mbed.h"
#include "MPU6050.h"

DigitalOut myled(LED1);
MPU6050 mpu(PB_3, PB_10);//(SDA,SCL)のピン配置
static BufferedSerial serial(PA_2, PA_3);

int main() {
    printf("Start\r\n");
        float filterCoefficient = 0.9; // ローパスフィルターの係数(これは環境によって要調整。1に近づけるほど平滑化の度合いが大きくなる。
        float lowpassValue = 0;
        float highpassValue = 0;
        float speed = 0;//加速度時から算出した速度
        float oldSpeed = 0;//ひとつ前の速度
        float oldAccel = 0;//ひとつ前の加速度
        float difference=0;//変位
        float timespan=0.01;//時間差
        int accel[3];//accelを3つの配列で定義。
    while(1){
        mpu.readAccelData(accel);//加速度の値をaccel[3]に代入
        int x = accel[0]-123;//x軸方向の加速度
        int y = accel[1]+60;//y軸方向の加速度
        int z = accel[2]+1110 ;//z軸方向の加速度
        float X = x*0.000597964111328125;
        float Y = y*0.000597964111328125;
        float Z = z*0.000597964111328125; 
        double a = X*X+Y*Y+Z*Z-95.982071137936;
        if (a>0){
        a = sqrt(a);
        }
        if (a<0) {
        a = abs(a);
        a = -sqrt(a);
        }
        //printf("%lf %f %f %f\r\n",a,X,Y,Z);
        // ローパスフィルター(現在の値 = 係数 * ひとつ前の値 ＋ (1 - 係数) * センサの値)
        lowpassValue = lowpassValue * filterCoefficient + a * (1 - filterCoefficient);
        // ハイパスフィルター(センサの値 - ローパスフィルターの値)//
        highpassValue = a - lowpassValue;

        // 速度計算(加速度を台形積分する)
        speed = ((highpassValue + oldAccel) * timespan) / 2 + speed;
        oldAccel = highpassValue;

        // 変位計算(速度を台形積分する)
        difference = ((speed + oldSpeed) * timespan) / 2 + difference;
        oldSpeed = speed;

        //printf(" speed %f difference %f\r\n",speed,difference);//速度と加速度を表示
        //printf("%f,",speed);//速度を表示
        printf("%d %d %d %lf  ",x,y,z,a);
        printf("speed %d diference %d\r\n",(int)speed,difference);//速度と変位を表示
        HAL_Delay(100);
    }
}