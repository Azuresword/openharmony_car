#include "main.h"
#include "serial_chassic.h"



extern int fd_imu;
// 转换波特率
speed_t conver_baudrate(int baudrate)
{
    switch (baudrate)
    {
        case 9600L:
            return B9600;
        case 19200L:
            return B19200;
        case 38400L:
            return B38400;
        case 115200L:
            return B115200;
        case 1152000L:
            return B1152000;
        default:
            return 1152000L;
    }
}

void set_baud(int fd_imu, int baud)
{
    int ret = ERR;
    struct termios opt;

    tcgetattr(fd_imu, &opt);    // tcgetattr用来获取终端参数，将从终端获得的信息fd_imu，保存到opt结构体中
    tcflush(fd_imu, TCIOFLUSH); // 刷清缓冲区
    cfsetispeed(&opt, baud);
    cfsetospeed(&opt, baud);

    ret = tcsetattr(fd_imu, TCSANOW, &opt); // 设置终端参数到opt中，使之立即生效
    if (ret == ERR)
    {
        perror("tcsetattr fd_imu");
        exit(0);
    }

    tcflush(fd_imu, TCIOFLUSH); // 刷清缓冲区
}

// 设置数据位
int setup_data_bits(int setup_databits, struct termios *options_databits)
{
    if (options_databits == NULL)
    {
        perror("setup_data_bits error");
        return ERR;
    }

    switch (setup_databits)
    {
        case 5L:
            options_databits->c_cflag |= CS5;
            break;
        case 6L:
            options_databits->c_cflag |= CS6;
            break;
        case 7L:
            options_databits->c_cflag |= CS7;
            break;
        case 8L:
            options_databits->c_cflag |= CS8;
            break;
        default:
            return ERR;
    }
    return OK;
}

// 设置校验位
int set_params_parity(int setup_parity, struct termios *options_parity)
{
    switch (setup_parity)
    {
        case 'n':
        case 'N':                               // 无奇偶校验位
            options_parity->c_cflag &= ~PARENB; // Clear parity enable/
            options_parity->c_iflag &= ~INPCK;  // disable input parity checking/
            break;

        case 'o':
        case 'O':                                         // 设置为奇校验
            options_parity->c_cflag |= (PARODD | PARENB); // odd parity checking
            options_parity->c_iflag |= INPCK;             // enable parity checking
            break;

        case 'e':
        case 'E':                               // 设置为偶校验
            options_parity->c_cflag |= PARENB;  // Enable parity /
            options_parity->c_cflag &= ~PARODD; // even parity/
            options_parity->c_iflag |= INPCK;   // enable parity checking /
            break;

        case 'M':
        case 'm': // 标记奇偶校验
            options_parity->c_cflag |= PARENB | CMSPAR | PARODD;
            options_parity->c_iflag |= INPCK; // enable parity checking /
            break;

        case 'S':
        case 's': // 设置为空格
            options_parity->c_cflag |= PARENB | CMSPAR;
            options_parity->c_cflag &= ~PARODD;
            options_parity->c_iflag |= INPCK; // enable parity checking /
            break;

        default:
            return ERR;
    }
    return OK;
}

// 设置校验位
int set_params(int fd_imu, int databits, int stopbits, int parity)
{
    struct termios options;
    int ret = ERR;

    if (tcgetattr(fd_imu, &options) != 0)
    {
        perror("tcgetattr fail\n");
        return ERR;
    }

    options.c_iflag = 0;
    options.c_oflag = 0;

    // setup data bits
    options.c_cflag &= ~CSIZE;
    ret = setup_data_bits(databits, &options);
    if (ret == ERR)
    {
        return ERR;
    }

    // parity
    ret = set_params_parity(parity, &options);
    if (ret == ERR)
    {
        return ERR;
    }

    // stop bits/
    switch (stopbits)
    {
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2L:
            options.c_cflag |= CSTOPB;
            break;
        default:
            return ERR;
    }

    // 请求发送和清除发送
    options.c_cflag &= ~CRTSCTS;
    options.c_lflag = 0;
    options.c_cc[VTIME] = 10L;
    options.c_cc[VMIN] = 1;

    tcflush(fd_imu, TCIFLUSH);
    if (tcsetattr(fd_imu, TCSANOW, &options) != 0)
    {
        return ERR;
    }

    return OK;
}

// 设置波特率
int uart_init(int fd_imu, int uartBaud)
{
    set_baud(fd_imu, conver_baudrate(uartBaud));
    // uart param /
    if (set_params(fd_imu, 8L, 1, 'n'))
    {
        perror("set uart parameters fail\n");
        return ERR;
    }
    return OK;
}

// 传感器数据处理
int data_proce(char *recv)
{
    while (*recv)
    {
        printf("%s", recv++);
    }
    return 0;
}



//***********************************//
// 串口发送控制测试


int my_uart_send(int vel, int yaw, int ang)
{

    printf("=======================================\r\n");
    printf("*************UART_example**************\r\n");

    // 通过串口2发送数据
    //  IoTUartWrite(2, (unsigned char *)send_data.send_data, 12);
    //  printf("Uart1 read data:");
    //  for (int i = 0; i < 12; i++)
    //  {
    //      printf(" %x", uart_buff[i]);
    //  }
    //  printf("\n");
    //  (void)memset(uart_buff, 0, sizeof(uart_buff));
    printf("\n=======================================\r\n");
    return 1;
}
void dec_to_hex(int vel, int yaw, int ang, unsigned char *send_data)
{
    int low;
    int high;
    low = vel & 0xff;
    high = (vel >>= 8) & 0xff;
    send_data[0] = (char)low;
    send_data[1] = (char)high;
    low = yaw & 0xff;
    high = (yaw >>= 8) & 0xff;
    send_data[2] = (char)low;
    send_data[3] = (char)high;
    low = ang & 0xff;
    high = (ang >>= 8) & 0xff;
    send_data[4] = (char)low;
    send_data[5] = (char)high;
}

// 获取校验位
void xor_cal(PC_SendType *pc_send)
{
    int temp = 0;
    temp ^= pc_send->data_length;
    temp ^= pc_send->response_id;
    for (int i = 0; i < pc_send->data_length; i++)
    {
        temp ^= pc_send->data[i];
    }
    pc_send->xor_num = temp;
}
// 获取发送数据
PC_SendType ChangeSendType(int vel, int yaw, int ang)
{
    PC_SendType SendData;
    memset(SendData.data, 0, 32);
    memset(SendData.send_data, 0, 40);

    SendData.header0 = 0X0A;
    SendData.header1 = 0X0C;
    SendData.data_length = 0X06;
    SendData.response_id = 0X02;
    // 参数保存入data
    dec_to_hex(vel, yaw, ang, SendData.data);
    // 获取校验位
    xor_cal(&SendData);
    // 保存入send_data
    SendData.send_data[0] = 0X0A;
    SendData.send_data[1] = 0X0C;
    SendData.send_data[2] = 0X06;
    SendData.send_data[3] = 0X00;
    SendData.send_data[4] = 0X02;
    for (int i = 5; i <= 10; i++)
    {
        SendData.send_data[i] = SendData.data[i - 5];
    }
    SendData.send_data[11] = SendData.xor_num;
    return SendData;
}

/****************************************************/
// 发送的数据结构体
typedef struct serial_data
{

    char databuf[100]; // 发送/接受数据
    int serfd;         // 串口文件描述符

} ser_Data;
ser_Data *snd = NULL;

// char buffer[] = {"hello my world!\r\002003600000



int main(int argc, char **argv)
{
    char *uart_dev = UART_TTL_NAME;
    int ret = ERR;
    pid.SetAngle=0;
    PID_init();
    fd_imu = open(uart_dev, O_RDWR);
    if (fd_imu == ERR)
    {
        perror("open file fail\n");
        return ERR;
    }
    ret = uart_init(fd_imu, 115200L);
    if (ret == ERR)
    {
        perror("uart init error\n");
        return ERR;
    }
//

    pthread_t pid_t1;
    pthread_t pid_t2;
    pthread_t pid_t3;
    pthread_t pid_t4;
    pthread_t pid_t5;
    pthread_t pid_t6;
    pthread_create(&pid_t5,NULL,(void *)_cpp_tcp_client,0);

    pthread_create(&pid_t1, NULL, (void *)_serial_output_task, 0);
    //pthread_create(&pid_t2, NULL, (void *)_serial_input_task, 0);
    pthread_create(&pid_t3, NULL, (void *)_imu_sensor_task, 0);
    //pthread_create(&pid_t4, NULL, (void *)_controller_task_, 0);

//    pthread_create(&pid_t5,NULL,(void *)_cpp_tcp_server,0);
    while (1)
    {
        sleep(10L);
    }
    close(fd_imu);

    return 0;
}