//
// Created by oh on 22-12-14.
//


#ifndef SERIAL_H
#define SERIAL_H


#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <linux/rtc.h>
#include "auto_scanner.h"


/************************************************************************************/

// 专用于IMU的串口驱动
// 调用wit sensor 端口查询程序SDK
void serial_close_imu(int fd);

int serial_open_imu(unsigned char *dev, unsigned int baud);

int serial_read_data_imu(int fd, unsigned char *val, int len);

int serial_write_data_imu(int fd, unsigned char *val, int len);

// 专用于毫米波雷达的串口驱动

#endif
