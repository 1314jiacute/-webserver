#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <thread>
#include <vector>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
int main(void) {
	//确定windows 网络编程版本 
	WORD ver = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(ver, &data);
	// 校验版本
	if (2 != HIBYTE(data.wVersion) || 2 != LOBYTE(data.wVersion))
	{
		// 版本不对
		cout << "确定windows编程版本失败" << endl;
		WSACleanup();
		return -1;
	}
	//创建一个套接字用于通信使用
	SOCKET cfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (cfd == -1) {
		cout << "创建套接字失败" << endl;
		WSACleanup();
		return -1;
	}
	cout << "创建套接字成功" << endl;
	sockaddr_in element = {};
	element.sin_family = AF_INET;
	element.sin_port = htons(9527);
	cout << "绑定端口为" << ntohs(element.sin_port) << endl;
	inet_pton(AF_INET, "127.0.0.1", &element.sin_addr.S_un.S_addr);
	int ret = connect(cfd, (sockaddr*)&element, sizeof(element));
	if (ret == -1) {
		cout << "连接失败" << endl;
		WSACleanup();
		return -1;
	}
	cout << "连接成功" << endl;
	while (1) {
		char message[1024];
		cin >> message;
		send(cfd, message, strlen(message)+1, 0);
		cout << "客户端发送信息" << endl;
		memset(message, 0, sizeof(message));
		int c_len = recv(cfd, message, sizeof(message), 0);
		if (c_len > 0) {
			cout << message << endl;
			
		}
		if (c_len == 0) {
			cout << "服务器断开连接" << endl;
			break;
		}
		if (c_len < 0) {
			cout << "客户端接收消息出问题 " <<endl;
			break;
		}
	}
	WSACleanup();
	closesocket(cfd);
	return 0;
}