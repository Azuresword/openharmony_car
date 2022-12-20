#include <iostream>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

using namespace std;

int main(int argv, char* argc[])
{
	//初始化
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建套接字
	SOCKET clntSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//向服务器发送消息
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));			//每个字节都用0填充
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(8888);
	connect(clntSock, (SOCKADDR*)& sockAddr, sizeof(SOCKADDR));
	char szBuffer[MAXBYTE] = { 0 };
	while (1) {
		//接收服务器消息
		
		//recv(clntSock, szBuffer, MAXBYTE, NULL);

		//输出接收到的数据
		//cout << "服务端：" << szBuffer << endl;

		//向服务端发送消息
		const char* str = "100";
		send(clntSock, str, strlen(str) + sizeof(char), NULL);
		Sleep(10);
	}
	//关闭套接字
	closesocket(clntSock);

	//终止dll
	WSACleanup();

	system("pause");

	return 0;
}

