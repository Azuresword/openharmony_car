//
// Created by oh on 22-12-14.
//

#ifndef CAR_PROJECT_PTHREAD_USR_H
#define CAR_PROJECT_PTHREAD_USR_H

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <unistd.h>


#include "serial.h"
#include "auto_scanner.h"
#include "controller.h"
#include "serial_chassic.h"
#include "main.h"
#include "socket_connection.h"
extern int IMU_INIT_FLAG;
extern int mode_select_FLAG;
void *_serial_input_task(void);
void *_serial_output_task(void);
void *_imu_sensor_task(void);
void *_controller_task_(void);
void *_cpp_tcp_client(void);
void *_cpp_tcp_server(void);

typedef struct sockaddr_in sockaddr_in;
typedef struct WSAData WSAData;
typedef struct sockaddr sockaddr;


#endif //CAR_PROJECT_PTHREAD_USR_H
