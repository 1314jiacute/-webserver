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
	//����һ���׽������ڽ���ʹ��
	//ȷ��ʹ�õ�����Э��
	SOCKET u_ls = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (u_ls==-1) {
		cout << "�����׽���ʧ��" << endl;
		WSACleanup();
		return -1;
	}
	cout << "�����׽��ֳɹ�" << endl;
	//���׽�����Ҫ�Ľṹ��
	sockaddr_in element = {};
	element.sin_family = AF_INET;
	element.sin_port = htons(9527);
	cout <<"�󶨶˿�Ϊ" << ntohs(element.sin_port) << endl;
	inet_pton(AF_INET,"127.0.0.1", &element.sin_addr.S_un.S_addr);
	int ret = bind(u_ls,(sockaddr*)&element, sizeof(element));
	if (ret == -1) {
		cout << "��ʧ��" << endl;
		WSACleanup();
		return -1;
	}
	cout << "�󶨳ɹ�" << endl;
	ret = listen(u_ls,128);
	if (ret == -1) {
		cout << " ����ʧ��" << endl;
		WSACleanup();
		return -1;
	}                    
	cout << "�����ɹ�" << endl;
	//������̬
	sockaddr_in  cadrr = {};
	int len  =  sizeof(cadrr) ;
	//����һ��ͨ���õ��׽���
	SOCKET cfd = accept(u_ls, (sockaddr*)&cadrr, &len);//���տͻ��˵�����
	if (cfd == -1) {
		cout << "������ϵʧ��" << endl;
		WSACleanup();
		return -1;
	}
	cout << " ������ϵ�ɹ�" << endl;
	char ip[32];
	cout << "�������������ϵ��ipΪ" << inet_ntop(AF_INET, &cadrr.sin_addr.S_un.S_addr, ip, sizeof(ip)) << "�󶨶˿�Ϊ" << ntohs(cadrr.sin_port) << endl;
	while (1) {
		char recall[1024];
		int lenth = sizeof(recall);
		int len = recv(cfd, recall, lenth, 0);// ����Ӧ��ʹ�õ����׽��ֱ��
		if (len > 0) {
			string se_s= "�������յ���\n";
			cout << "����˵:" << recall<< endl;
			send(cfd, se_s.c_str(), se_s.size(), 0);
		}
		else if (len == 0) {
			cout << "�Է��Ͽ�����" << endl;
			break;
		}
		 else if (len < 0) {
			cout << "������Ϣʧ��" << endl;
			break;
		}

	}
	closesocket(u_ls);
	closesocket(cfd);
	WSACleanup();
	return 0;
}