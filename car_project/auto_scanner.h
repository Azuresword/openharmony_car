//
// Created by oh on 22-12-14.
//

#ifndef CAR_PROJECT_AUTO_SCANNER_H
#define CAR_PROJECT_AUTO_SCANNER_H

#include "serial.h"
#include "wit_c_sdk.h"
#include "REG.h"
#include <stdint.h>

#define ACC_UPDATE 0x01
#define GYRO_UPDATE 0x02
#define ANGLE_UPDATE 0x04
#define MAG_UPDATE 0x08
#define READ_UPDATE 0x80

#define UART_SENSOR_NAME "/dev/ttyUSB0"

extern int fd, s_iCurBaud;
extern volatile char s_cDataUpdate;

void AutoScanSensor(char *dev);
void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum);
void Delayms(uint16_t ucMs);

#endif //CAR_PROJECT_AUTO_SCANNER_H
