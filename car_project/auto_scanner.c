//
// Created by oh on 22-12-14.
//

#include "auto_scanner.h"


int c_uiBaud[] = {2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};

void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum)
{
    int i;
    for (i = 0; i < uiRegNum; i++)
    {
        switch (uiReg)
        {
            //            case AX:
            //            case AY:
            case AZ:
                s_cDataUpdate |= ACC_UPDATE;
                break;
                //            case GX:
                //            case GY:
            case GZ:
                s_cDataUpdate |= GYRO_UPDATE;
                break;
                //            case HX:
                //            case HY:
            case HZ:
                s_cDataUpdate |= MAG_UPDATE;
                break;
                //            case Roll:
                //            case Pitch:
            case Yaw:
                s_cDataUpdate |= ANGLE_UPDATE;
                break;
            default:
                s_cDataUpdate |= READ_UPDATE;
                break;
        }
        uiReg++;
    }
}

void Delayms(uint16_t ucMs)
{
    usleep(ucMs * 1000);
}

void AutoScanSensor(char *dev)
{
    int i, iRetry;
    char cBuff[1];

    for (i = 1; i < 10; i++)
    {
        serial_close_imu(fd);
        s_iCurBaud = c_uiBaud[i];
        fd = serial_open_imu(dev, c_uiBaud[i]);

        iRetry = 2;
        do
        {
            s_cDataUpdate = 0;
            WitReadReg(AX, 3);
            Delayms(200);
            while (serial_read_data_imu(fd, cBuff, 1))
            {
                WitSerialDataIn(cBuff[0]);
            }
            if (s_cDataUpdate != 0)
            {
                printf("%d baud find sensor\r\n\r\n", c_uiBaud[i]);
                return;
            }
            iRetry--;
        } while (iRetry);
    }
    printf("can not find sensor\r\n");
    printf("please check your connection\r\n");
}

