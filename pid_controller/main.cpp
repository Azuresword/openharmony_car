#include <stdio.h>

struct pid{
    float SetAngle;          //定义设定值
    float ActualAngle;        //定义实际值
    float err;                //定义偏差值
    float err_last;            //定义上一个偏差值
    float Kp,Ki,Kd;            //定义比例、积分、微分系数
    float speed;          //定义电压值（控制执行器的变量）
    float integral;            //定义积分值
    float out;
}pid;


void PID_init() {
    printf("PID_init begin \n");
    pid.SetAngle = 0.0;
    pid.ActualAngle = 0.0;
    pid.err = 0.0;
    pid.err_last = 0.0;
    pid.speed = 0.0;
    pid.integral = 0.0;
    pid.Kp = 0.2;
    pid.Ki = 0.015;
    pid.Kd = 0.2;
    printf("PID_init end \n");
}

float PID_realize(float angle,float ActualAngle){
    pid.SetAngle=angle;
    pid.ActualAngle=ActualAngle;
    pid.err= pid.SetAngle - pid.ActualAngle;
    pid.integral+=pid.err;
    pid.speed= pid.Kp * pid.err + pid.Ki * pid.integral + pid.Kd * (pid.err - pid.err_last);
    pid.err_last=pid.err;
    pid.out=pid.speed*1.0f;
    return pid.out;
}

int main(){
    printf("System begin \n");
    PID_init();
    //测试用例
  //  float actang[15]={1,4,20,50,100,250,300,200,200,200,200,200,200,200,200};
   float act;
   float target=200.0;
    int count=0;
    while(count<15)
    {
        float speed=PID_realize(target,act);
        printf("%f\n",speed);
        count++;
    }
    return 0;
}
