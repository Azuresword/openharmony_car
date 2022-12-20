//
// Created by oh on 22-12-14.
//

#include "controller.h"

pid_res pid;

void PID_init(){
    printf("PID_init begin \n");
    pid.SetAngle = 0.0;
    pid.ActualAngle = 0.0;
    pid.err = 0.0;
    pid.err_last = 0.0;
    pid.speed = 0.0;
    pid.integral = 0.0;
    pid.Kp = 0.4;
    pid.Ki = 0.000;
    pid.Kd = 0.001;
    pid.limits=250;
    pid.out=0;
    printf("PID_init end \n");
   // printf("PID_init begin \n");
   // pid.SetAngle=0.0;
   // pid.ActualAngle=0.0;
   // pid.err=0.0;
   // pid.err_last=0.0;
   // pid.integral=0.0;
   // pid.angle= 0.0;
   // pid.Kp=0.2;
   // pid.Ki=0.015;
   // pid.Kd=0.2;
   // pid.limit=250;
   // printf("PID_init end \n");
}

float pid_pos_update(pid_res *p){

    float out =0;
    //pid.ActualAngle=Act_ang;
    //计算当前误差
    p->err =p->SetAngle-p->ActualAngle;
    //误差积分
    p->integral+=p->err;
    //pid公式
    pid.speed= pid.Kp * pid.err + pid.Ki * pid.integral + pid.Kd * (pid.err - pid.err_last);
    //误差迭代
    p->err_last=p->err;
    //输出
    out=pid.speed*1.0f;
    //限幅s
    if(out>=0)out+=90;
    else out-=90;
    out = range(out, -p->limits, p->limits);

    pid.out=out;
    return out;
}
double  velocityx[1];
double positionX[1];
double ay_0;
double ay_1;
float double_integrating_acc(double ay_0,double ay_1){
     //first integration

     velocityx[1]= velocityx[0]+ ay_0+(ay_1- ay_0)/2;
     //second integration
     positionX[1]= positionX[0]+ velocityx[0]+((velocityx[1]- velocityx[0])/2);
     return  positionX[1];
}

void mode_1(){
    //turn;
    printf("setangle=%lf",pid.SetAngle);
    pid_pos_update(&pid);
    printf("PID out dead at %f\n", pid.out);
    send_data = ChangeSendType(0, 0, pid.out);
}

void mode_2(){
    //forward move
    ay_1=integral_length.Actacc;
    integral_length.Actuallength=double_integrating_acc(ay_1,ay_0);
    ay_0=ay_1;
    send_data = ChangeSendType(200, 0, 0);
}