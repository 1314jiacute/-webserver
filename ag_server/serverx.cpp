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
	//创建一个套接字用于接听使用
	//确定使用的网络协议
	SOCKET u_ls = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (u_ls==-1) {
		cout << "创建套接字失败" << endl;
		WSACleanup();
		return -1;
	}
	cout << "创建套接字成功" << endl;
	//绑定套接字需要的结构体
	sockaddr_in element = {};
	element.sin_family = AF_INET;
	element.sin_port = htons(9527);
	cout <<"绑定端口为" << ntohs(element.sin_port) << endl;
	inet_pton(AF_INET,"127.0.0.1", &element.sin_addr.S_un.S_addr);
	int ret = bind(u_ls,(sockaddr*)&element, sizeof(element));
	if (ret == -1) {
		cout << "绑定失败" << endl;
		WSACleanup();
		return -1;
	}
	cout << "绑定成功" << endl;
	ret = listen(u_ls,128);
	if (ret == -1) {
		cout << " 监听失败" << endl;
		WSACleanup();
		return -1;
	}                    
	cout << "监听成功" << endl;
	//设置阻态
	sockaddr_in  cadrr = {};
	int len  =  sizeof(cadrr) ;
	//创建一个通信用的套接字
	SOCKET cfd = accept(u_ls, (sockaddr*)&cadrr, &len);//接收客户端的请求
	if (cfd == -1) {
		cout << "建立联系失败" << endl;
		WSACleanup();
		return -1;
	}
	cout << " 建立联系成功" << endl;
	char ip[32];
	cout << "与服务器建立联系的ip为" << inet_ntop(AF_INET, &cadrr.sin_addr.S_un.S_addr, ip, sizeof(ip)) << "绑定端口为" << ntohs(cadrr.sin_port) << endl;
	while (1) {
		char recall[1024];
		int lenth = sizeof(recall);
		int len = recv(cfd, recall, lenth, 0);// 这里应该使用的是套接字编程
		if (len > 0) {
			string se_s= "服务器收到了\n";
			cout << "有人说:" << recall<< endl;
			send(cfd, se_s.c_str(), se_s.size(), 0);
		}
		else if (len == 0) {
			cout << "对方断开连接" << endl;
			break;
		}
		 else if (len < 0) {
			cout << "接收消息失败" << endl;
			break;
		}

	}
	closesocket(u_ls);
	closesocket(cfd);
	WSACleanup();
	return 0;
}