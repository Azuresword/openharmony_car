//
// Created by oh on 22-12-14.
//

#ifndef CAR_PROJECT_MAIN_H
#define CAR_PROJECT_MAIN_H

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "controller.h"
#include "serial.h"
#include "pthread_usr.h"
#include "serial_chassic.h"
#include "socket_connection.h"
typedef struct
{
    char header0;
    char header1;
    short data_length;
    char response_id;
    unsigned char data[32];
    unsigned char xor_num;
    unsigned char send_data[40];
} PC_SendType;

// 获取发送数据
PC_SendType ChangeSendType(int vel, int yaw, int ang);
PC_SendType send_data;
#endif //CAR_PROJECT_MAIN_H
