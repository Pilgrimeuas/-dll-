/*
* ���ùܱ���ֱ�Ӹ��Ƶ�modulemanager.cpp
����ŵ�Modulemanager.cpp����ִ��һ�ξ�����
*/

#include <winsock2.h>  
#include <stdio.h>  
#include <string>
using namespace std;
#pragma comment(lib,"ws2_32.lib")	//��ws2_32.lib�ӵ�Linkҳ�����ӿ�  
#define IP "1.1.1.1"			//����̨������ϲ��ԣ�IPΪServer�˵�IP��ַ �ǵ����ã�����
#define PORT 19132					//ע�⣺�ͻ�������ͨ�ŵĶ˿� = ����˵Ķ˿�
#define BUFFER_SIZE 1024			//���ݷ��ͻ�������С
HW_PROFILE_INFO hwProfileInfo;
bool Go = false;
bool first = false;

void Veri() {

	if (g_Data.getLocalPlayer() == nullptr)
		return;

	auto guiData = g_Data.getClientInstance()->getGuiData();

	std::wstring hardware_guid;
	GetCurrentHwProfile(&hwProfileInfo);
	hardware_guid = hwProfileInfo.szHwProfileGuid;
	std::string hwid(hardware_guid.begin(), hardware_guid.end());//��һ���ǻ�ȡhwid
	//תΪ�ַ���

	char buf[1024];//buf�����ſͻ��˷��͵���Ϣ  
	char Rec[1024];
	int i;				
	for (i = 0; i < hwid.length(); i++)
		buf[i] = hwid[i];
	buf[i] = '\0';						//��stringת¼��char,��֪��Ϊʲô��c_str����

	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) == SOCKET_ERROR)  //WSAStartup()������Winsock DLL���г�ʼ��
	{GameData::terminate();//��ֹ��Ϸ
		return;
	}
	SOCKET sock;											//�ͻ��˽��̴����׽���
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)  //�������׽��֣������˱���һ�£�
	{
		GameData::terminate();
		WSACleanup();
		return;
	}

	struct sockaddr_in ClientAddr;				//sockaddr_in�ṹ������ʶTCP/IPЭ���µĵ�ַ����ǿ��ת��Ϊsockaddr�ṹ
	ClientAddr.sin_family = AF_INET;				//ָInternet��
	ClientAddr.sin_port = htons(PORT);			//ָ���������Ԥ���Ķ˿�
	ClientAddr.sin_addr.s_addr = inet_addr(IP);	//ָ����������󶨵�IP��ַ
	if (connect(sock, (LPSOCKADDR)&ClientAddr, sizeof(ClientAddr)) == SOCKET_ERROR)  //����connect()����������������̷�����������  
	{
		GameData::terminate();
		closesocket(sock);
		WSACleanup();
		return;
	}

	if (!Go && !first) {
		send(sock, buf, 1024, 0);				 //�����������hwid
		Go = true;
	}

	/*��������*/
	if (Go && !first) {
		memset(Rec, 0, sizeof(Rec));//��ʼ��
		recv(sock, Rec, 1024, 0);//�������Է��������ź�



		if (!strncmp("123", Rec, 4))//123�Ƿ���˵ĳɹ��źţ��Լ�����
		{
			if (g_Data.getLocalPlayer() != nullptr)
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sVerify!", WHITE);
			//first = true;
		}

		else {
			if (g_Data.getLocalPlayer() != nullptr) {
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sYou are not Verified", WHITE);
				GameData::terminate();
				Go = false;
				first = false;
			}
		}
	}
	closesocket(sock);							 //�ر��׽���
	WSACleanup();//��ֹ��Winsock DLL��ʹ�ã����ͷ���Դ���Ա���һ��ʹ��

}
