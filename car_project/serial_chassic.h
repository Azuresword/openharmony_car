//
// Created by oh on 22-12-14.
//

#ifndef CAR_PROJECT_SERIAL_CHASSIC_H
#define CAR_PROJECT_SERIAL_CHASSIC_H


#define UART_TTL_NAME "/dev/ttyUSB1"
#define FRAME_LEN 4
#define RECV_HEAD 0xaa
#define RECV_END 0x55
#define EVT_RIGHT 0x01
#define EVT_LEFT 0x02
#define EVT_BACK 0x03
#define EVT_FORWARD 0x04
#define EVT_PULLUP 0x05
#define EVT_PULLDOWN 0x06
#define EVT_PULLREMOVE 0x07
#define EVT_TOUCH1 0x21
#define EVT_TOUCH2 0x22
#define EVT_TOUCH3 0x23
#define EVT_TOUCH4 0x24
#define EVT_TOUCH5 0x25
#define OK 0
#define ERR (-1)

#define B0 0000000 /* hang up */
#define B50 0000001
#define B75 0000002
#define B110 0000003
#define B134 0000004
#define B150 0000005
#define B200 0000006
#define B300 0000007
#define B600 0000010
#define B1200 0000011
#define B1800 0000012
#define B2400 0000013
#define B4800 0000014
#define B9600 0000015
#define B19200 0000016
#define B38400 0000017
#ifdef __USE_MISC
#define EXTA B19200
#define EXTB B38400
#endif
#define CSIZE 0000060 /*数据位屏蔽*/
#define CS5 0000000   /*5,6,7,8为数据位*/
#define CS6 0000020
#define CS7 0000040
#define CS8 0000060
#define CSTOPB 0000100 /*停止位*/
#define CREAD 0000200  /*接收标志位*/
#define PARENB 0000400 /*奇偶校验位开启标志位*/
#define PARODD 0001000 /*奇校验，否则偶校验*/
#define HUPCL 0002000
#define CLOCAL 0004000 /*本地连接标志位*/

#define B57600 0010001
#define B115200 0010002
#define B230400 0010003
#define B460800 0010004
#define B500000 0010005
#define B576000 0010006
#define B921600 0010007
#define B1000000 0010010
#define B1152000 0010011
#define B1500000 0010012
#define B2000000 0010013
#define B2500000 0010014
#define B3000000 0010015
#define B3500000 0010016
#define B4000000 0010017
#define __MAX_BAUD B4000000

#define CIBAUD 002003600000  /* input baud rate (not used) */
#define CMSPAR 010000000000  /* mark or space (stick) parity */
#define CRTSCTS 020000000000 /* flow control */

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
int uart_init(int fd_imu, int uartBaud);
int data_proce(char *recv);
#endif //CAR_PROJECT_SERIAL_CHASSIC_H
