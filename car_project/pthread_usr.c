//
// Created by oh on 22-12-14.
//
#include "pthread_usr.h"

int IMU_INIT_FLAG=0;
int mode_select_FLAG=0;
integral_acc integral_length;
pid_res pid;
int fd_imu;
int fd, s_iCurBaud = 9600;
float Act_ang;
float angle_before;

volatile char s_cDataUpdate = 0;
void itostr(char*, int);
void reverse(char*);
void *_imu_sensor_task(void)
{
    char *uart_dev = UART_SENSOR_NAME;
    if ((fd = serial_open_imu(uart_dev, 115200) < 0))
    {
        printf("open fail\n");
        return 0;
    }
    else
        printf("open success\n");

    float fAcc[3], fGyro[3], fAngle[3];
    int i, ret;
    char cBuff[1];

    WitInit(WIT_PROTOCOL_NORMAL, 0x50);
    WitRegisterCallBack(SensorDataUpdata);

    printf("\r\n********************** wit-motion Normal example  ************************\r\n");
     AutoScanSensor(uart_dev);

    while (1)
    {
        IMU_INIT_FLAG=1;
        Delayms(5);

        while (serial_read_data_imu(fd, cBuff, 1))
        {
            WitSerialDataIn(cBuff[0]);
        }
        printf("\n");
        Delayms(5);

        if (s_cDataUpdate)
        {
            for (i = 0; i < 3; i++)
            {
                fAcc[i] = sReg[AX + i] / 32768.0f * 16.0f;
                fGyro[i] = sReg[GX + i] / 32768.0f * 2000.0f;
                fAngle[i] = sReg[Roll + i] / 32768.0f * 180.0f;
            }

            if (s_cDataUpdate & ACC_UPDATE)
            {
                printf("acc:%.3f %.3f %.3f\r\n", fAcc[0], fAcc[1], fAcc[2]);
                integral_length.Actacc = fAcc[0]*9.8;
                s_cDataUpdate &= ~ACC_UPDATE;
            }
            if (s_cDataUpdate & GYRO_UPDATE)
            {
                printf("gyro:%.3f %.3f %.3f\r\n", fGyro[0], fGyro[1], fGyro[2]);
                s_cDataUpdate &= ~GYRO_UPDATE;
            }
            if (s_cDataUpdate & ANGLE_UPDATE)
            {
                pid.ActualAngle=fAngle[2]+180;
                printf("act=%lf",pid.ActualAngle);
                printf("angle:%.3f %.3f %.3f\r\n", fAngle[0], fAngle[1], fAngle[2]);
                s_cDataUpdate &= ~ANGLE_UPDATE;
            }
            if (s_cDataUpdate & MAG_UPDATE)
            {
                printf("mag:%d %d %d\r\n", sReg[HX], sReg[HY], sReg[HZ]);
                s_cDataUpdate &= ~MAG_UPDATE;
            }


        }
    }

    serial_close_imu(fd);
}

void *_controller_task_(void)
{
    float out;
    out=pid_pos_update(&pid);
    printf("pid output is %f",out);
}


void *_serial_input_task(void)
{
    int i = 0;
    int ret = ERR;
    int buf = 0;
    char recv[FRAME_LEN] = {0};
    pthread_detach(pthread_self());
    printf("test _ready\n");
    while (1)
    {
        for (i = 0; i < FRAME_LEN; i++)
        {
            ret = read(fd_imu, &buf, 1);
            if (ret == ERR)
            {
                perror("read error\n");
                exit(0);
            }
            recv[i] = buf;
        }
        ret = data_proce(recv);
        if (ret == ERR)
        {
            perror("data process error\n");
            exit(0);
        }
    }
}


void *_serial_output_task(void) {

    pthread_detach(pthread_self());
    // memcpy(snd->databuf,buffer,strlen(buffer));//拷贝发送数据
    int ret;
    // for(int i=0;i<10;i++) {
    //   write(fd_imu, (unsigned char *) "0x0a,0x0c,0x01,0x00,0x01,0x01,0x01,", 7);
    // usleep(300);
    //}
    while (1) {

        // my_uart_send(0, 0, 500);
        if (IMU_INIT_FLAG == 1) {
            if (mode_select_FLAG == 0) {
                mode_1();
                if (pid.ActualAngle > pid.SetAngle - 5 && pid.ActualAngle < pid.SetAngle +5) {
                    mode_select_FLAG = 1;
                    double temp = pid.SetAngle;
                    velocityx[0] = 0;
                    positionX[0] = 0;
                    velocityx[1] = 0;
                    positionX[1] = 0;
                    pid.SetAngle = temp;
                    usleep(3000000);
                }
            }
            else if (mode_select_FLAG == 1) {
                integral_length.Setlength=0.5;
                double temp = pid.SetAngle;
                mode_2();
                pid.SetAngle = temp;
                printf("act_length=%f\n",integral_length.Actuallength);
                if (integral_length.Actuallength <integral_length.Setlength+0.05&&integral_length.Actuallength >integral_length.Setlength-0.05) {
                    mode_select_FLAG = 0;
                    pid.SetAngle+=90;
                    if(pid.SetAngle>360)pid.SetAngle=pid.SetAngle-360;
                    usleep(3000000);

                }

            }
           // pid.SetAngle = 180;
           // pid_pos_update(&pid);
           // printf("PID out dead at %f\n", pid.out);
           // send_data = ChangeSendType(0, 0, pid.out);
           //mode_1();
           // send_data = ChangeSendType(0, 0, pid.out);
            // IoTUartWrite(2, (unsigned char *)send_data.send_data, 12);
            ret = write(fd_imu, (unsigned char *) send_data.send_data, 12);
            if (ret > 0) {
                // printf("send success, data is  %s\r\n",snd->databuf);
            } else {
                printf("send error!\r\n");
            }
            usleep(100000);
        }
    }

}
    void *_cpp_tcp_client(void) {
// 1. 创建客户端，并连接到服务端
        int sock_client = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_addr.s_addr = inet_addr("172.20.10.1");
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(8888);
        connect(sock_client, (sockaddr *) &server_addr, sizeof(sockaddr));

        // 2. 发送数据，并接受服务端数据
        char send_info[256] = {0};

        //int n = -129;
        //char nums[10];
        //itostr(nums, n);
        // printf("%s\n", nums);
        int out_tcp_client;
        while (1) {
            if (IMU_INIT_FLAG == 0) {
                sleep(10);
                continue;
            }
          //  if(pid.out>0)out_tcp_client=pid.out-60;
          //  else out_tcp_client=pid.out+60;
            itostr(send_info, (int) pid.ActualAngle);

            send(sock_client, send_info, strlen(send_info) + 1, 0);
            printf("pid_out:  %f,send:    %s\n", pid.out, send_info);
            usleep(3000);
            //char recv_info[50];
            //recv(sock_client, recv_info, sizeof(recv_info), 0);
            //printf("receive: %s\n", recv_info);
            memset(send_info, '\0', 256);
        }

        // 3. 关闭客户端
        close(sock_client);
        return 0;

    }
    void *_cpp_tcp_server(void) {
/**
   * 1. 创建服务端socket，并绑定相应ip和端口
   *    SOCK_STREAM对应的是TCP协议，安全可靠；SOCK_DGRAM是UDP协议，不可靠
   *    listen使得该进程可以接收socket的请求，成为一个服务端。对应的是客户端的connect。
  */
        int sock_server = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(8888);
        bind(sock_server, (struct sockaddr *) &server_addr, sizeof(server_addr));
        listen(sock_server, 5);

        // 2. 服务端接受客户端的请求
        int socklen = sizeof(struct sockaddr_in);
        sockaddr_in client_addr;
        int sock_client = accept(sock_server, (struct sockaddr *) &client_addr,
                                 (socklen_t *) &socklen);
        printf("client %s has connnected\n", inet_ntoa(client_addr.sin_addr));

        // 3. 同客户端连接，并接受数据
        char buffer[50];
        while (1) {
            memset(buffer, 0, sizeof(buffer));
            recv(sock_client, buffer, sizeof(buffer), 0);
            printf("receive: %s\n", buffer);

            strcpy(buffer, "I'm fine, thx!");
            send(sock_client, buffer, strlen(buffer), 0);
            printf("send   : %s\n", buffer);
        }
        // 4. 关闭
        close(sock_server);
        close(sock_client);
        return 0;

    }


    void itostr(char *str, int n) {
        int i = 0;
        if (n < 0) {
            str[0] = '-';
            i++;
        }
        n = n < 0 ? -n : n;
        while (n > 0) {
            str[i] = n % 10 + '0';
            n /= 10;
            i++;
        }
        if (*str == '-')
            reverse(str + 1);
        else
            reverse(str);
    }

    void reverse(char *str) //reverse函数功能为反转字符串
    {
        char *t;
        int c;
        t = str + (strlen(str)) - 1; //记录字符串末尾的内存地址
        for (t; str < t; str++, t--)//str为第一个地址向右移动，t为末始地址向左移动，利用循环将字符串反转
        {
            c = *str; // 将str当前地址对应的内容赋值给c
            *str = *t; //将当前地址t中的内容赋值给str
            *t = c; //将c的内容赋值给*t
        }
    }
