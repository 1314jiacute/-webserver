#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <thread>
#include <vector>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
int main(void) {
	//ȷ��windows �����̰汾 
	WORD ver = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(ver, &data);
	// У��汾
	if (2 != HIBYTE(data.wVersion) || 2 != LOBYTE(data.wVersion))
	{
		// �汾����
		cout << "ȷ��windows��̰汾ʧ��" << endl;
		WSACleanup();
		return -1;
	}
	//����һ���׽�������ͨ��ʹ��
	SOCKET cfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (cfd == -1) {
		cout << "�����׽���ʧ��" << endl;
		WSACleanup();
		return -1;
	}
	cout << "�����׽��ֳɹ�" << endl;
	sockaddr_in element = {};
	element.sin_family = AF_INET;
	element.sin_port = htons(9527);
	cout << "�󶨶˿�Ϊ" << ntohs(element.sin_port) << endl;
	inet_pton(AF_INET, "127.0.0.1", &element.sin_addr.S_un.S_addr);
	int ret = connect(cfd, (sockaddr*)&element, sizeof(element));
	if (ret == -1) {
		cout << "����ʧ��" << endl;
		WSACleanup();
		return -1;
	}
	cout << "���ӳɹ�" << endl;
	while (1) {
		char message[1024];
		cin >> message;
		send(cfd, message, strlen(message)+1, 0);
		cout << "�ͻ��˷�����Ϣ" << endl;
		memset(message, 0, sizeof(message));
		int c_len = recv(cfd, message, sizeof(message), 0);
		if (c_len > 0) {
			cout << message << endl;
			
		}
		if (c_len == 0) {
			cout << "�������Ͽ�����" << endl;
			break;
		}
		if (c_len < 0) {
			cout << "�ͻ��˽�����Ϣ������ " <<endl;
			break;
		}
	}
	WSACleanup();
	closesocket(cfd);
	return 0;
}