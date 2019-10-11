#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include "..\..\Common\FIND_HUMAN_PACKET_HEADER.h"
#include <thread>
#include <vector>

#define SERVERPORT 9000
#define BUFSIZE    1024

// ���� ���� ������ ���� ����ü
struct SOCKETINFO
{
	OVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
	WSABUF wsabuf;
};

class USER_INFO
{
public:
	int index;
	char szBuf[BUFSIZE];
	int len;
	int roomNumber;
	char name[32];
	char nameLen;
};

class ROOM_INFO
{
public:
	int playerNum = 0;
	std::vector <SOCKETINFO*> vecUserSocket;
};

int g_iIndex = 0;
std::map<SOCKETINFO*, USER_INFO*> g_mapUser;
ROOM_INFO g_arrRoom[11];

// �۾��� ������ �Լ�
DWORD WINAPI WorkerThread(LPVOID arg);
//test func
bool ProcessPacket(SOCKETINFO* ptr, USER_INFO* pUser, DWORD &len);
// ���� ��� �Լ�
void err_quit(const char *msg);
void err_display(const char *msg);

void sendRoomExit(SOCKETINFO* ptr);

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	// ����� �Ϸ� ��Ʈ ����
	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL) return 1;

	// CPU ���� Ȯ��
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	// (CPU ���� * 2)���� �۾��� ������ ����
	HANDLE hThread;
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; i++) {
		hThread = CreateThread(NULL, 0, WorkerThread, hcp, 0, NULL);
		if (hThread == NULL) return 1;
		CloseHandle(hThread);
	}

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	DWORD recvbytes, flags;

	while (1)
	{
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) 
		{
			err_display("accept()");
			break;
		}
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// ���ϰ� ����� �Ϸ� ��Ʈ ����
		CreateIoCompletionPort((HANDLE)client_sock, hcp, client_sock, 0);

		// ���� ���� ����ü �Ҵ�
		SOCKETINFO *ptr = new SOCKETINFO;
		if (ptr == NULL) break;
		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
		ptr->sock = client_sock;
		ptr->recvbytes = ptr->sendbytes = 0;
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = BUFSIZE;

		//�α�����Ŷ ����
		USER_INFO* pInfo = new USER_INFO();
		pInfo->index = g_iIndex++;
		pInfo->len = 0;
		g_mapUser.insert(std::make_pair(ptr, pInfo));

		// �񵿱� ����� ����
		flags = 0;
		retval = WSARecv(client_sock, &ptr->wsabuf, 1, &recvbytes,
			&flags, &ptr->overlapped, NULL);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING) {
				err_display("WSARecv()");
			}
			continue;
		}
	}

	// ���� ����
	WSACleanup();
	return 0;
}

//�۾��� ������ �Լ�
DWORD WINAPI WorkerThread(LPVOID arg)
{
	int retval;
	HANDLE hcp = (HANDLE)arg;

	while (1)
	{
		//std::cout << std::this_thread::get_id() << std::endl;
		//�񵿱� ����� �Ϸ� ��ٸ���
		DWORD cbTransferred;
		SOCKET client_sock;
		SOCKETINFO *ptr;
		retval = GetQueuedCompletionStatus(hcp, &cbTransferred,
			(PULONG_PTR)&client_sock, (LPOVERLAPPED *)&ptr, INFINITE);

		//Ŭ���̾�Ʈ ���� ���
		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);

		//�񵿱� ����� ��� Ȯ��
		if (retval == 0 || cbTransferred == 0)
		{
			if (retval == 0) {
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &ptr->overlapped,
					&temp1, FALSE, &temp2);
				err_display("WSAGetOverlappedResult()");
			}
			//�濡 �ִ� �������� �����ϰ�
			for (auto iter = g_arrRoom[g_mapUser[ptr]->roomNumber].vecUserSocket.begin();
				iter != g_arrRoom[g_mapUser[ptr]->roomNumber].vecUserSocket.end(); iter++)
			{
				if ((*iter)->sock == ptr->sock)
				{
					g_arrRoom[g_mapUser[ptr]->roomNumber].playerNum--;
					g_arrRoom[g_mapUser[ptr]->roomNumber].vecUserSocket.erase(iter);
					break;
				}
			}
			//������ �����ְ�
			sendRoomExit(ptr);
			//���� ���� ���� �����ϰ�
			g_mapUser.erase(ptr);
			//���ϴݰ�
			closesocket(ptr->sock);
			printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
			delete ptr;
			continue;
		}

		// ������ �ޱ�
		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = BUFSIZE;

		USER_INFO* pUser = g_mapUser[ptr];

		while (true)
		{
			if (!ProcessPacket(ptr, pUser, cbTransferred))
			{
				Sleep(100);
				//SendMessage(hWnd, uMsg, wParam, lParam);
				break;
			}
			else
			{
				if (pUser->len < sizeof(PACKET_HEADER))
					break;
			}
		}

		DWORD recvbytes;
		DWORD flags = 0;
		retval = WSARecv(ptr->sock, &ptr->wsabuf, 1,
			&recvbytes, &flags, &ptr->overlapped, NULL);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING) {
				err_display("WSARecv()");
			}
			continue;
		}

	}
	return 0;
}

bool ProcessPacket(SOCKETINFO* ptr, USER_INFO* pUser, DWORD &len)
{
	if (len > 0)
	{
		memcpy(&pUser->szBuf[pUser->len], ptr->buf, len);
		pUser->len += len;
		len = 0;
	}

	if (pUser->len < sizeof(PACKET_HEADER))
		return false;

	PACKET_HEADER header;
	memcpy(&header, pUser->szBuf, sizeof(header));

	if (pUser->len < header.wLen)
		return false;

	switch (header.wIndex)
	{
	case PACKET_INDEX_SEND_USERNAME:
	{
		PACKET_SEND_USERNAME packet;
		memcpy(&packet, ptr->buf, header.wLen);

		g_mapUser[ptr]->nameLen = packet.nameLen;
		for (int i = 0; i < packet.nameLen; i++)
		{
			g_mapUser[ptr]->name[i] = packet.name[i];
		}
		g_mapUser[ptr]->name[packet.nameLen] = '\0';

		g_mapUser[ptr]->roomNumber = 0;//�κ� ����

		g_arrRoom[g_mapUser[ptr]->roomNumber].playerNum++;
		g_arrRoom[g_mapUser[ptr]->roomNumber].vecUserSocket.push_back(ptr);
		
		send(ptr->sock, (const char*)&packet, header.wLen, 0);
		
	}
	break;
	case PACKET_INDEX_SEND_LOGIN_OK:
	{
		PACKET_SEND_LOGIN_OK packet;
		memcpy(&packet, ptr->buf, header.wLen);
		std::cout << "user : " << g_mapUser[ptr]->name <<" Login Ok" <<std::endl;

		send(ptr->sock, (const char*)&packet, header.wLen, 0);
		
	}
	break;
	case PACKET_INDEX_SEND_CHAT:
	{
		PACKET_SEND_CHAT packet;
		memcpy(&packet, pUser->szBuf, header.wLen);

		packet.nameLen = g_mapUser[ptr]->nameLen;
		for (int i = 0; i < g_mapUser[ptr]->nameLen; i++)
		{
			packet.name[i] = g_mapUser[ptr]->name[i];
		}
		packet.name[g_mapUser[ptr]->nameLen] = '\0';

		std::cout << g_mapUser[ptr]->name << " : send chatMsg" << std::endl;
		for (auto iter = g_arrRoom[g_mapUser[ptr]->roomNumber].vecUserSocket.begin(); 
			iter != g_arrRoom[g_mapUser[ptr]->roomNumber].vecUserSocket.end(); iter++)
		{
			send( (*iter)->sock , (const char*)&packet, header.wLen, 0);
		}
	}
	break;
	case PACKET_INDEX_SEND_USER_VIEW:
	{
		PACKET_SEND_USER_VIEW packet;
		packet.header.wIndex = PACKET_INDEX_SEND_USER_VIEW;
		packet.len = 0;
		int totalLen = 0;
		for (auto iter = g_arrRoom[g_mapUser[ptr]->roomNumber].vecUserSocket.begin();
			iter != g_arrRoom[g_mapUser[ptr]->roomNumber].vecUserSocket.end(); iter++)
		{
			packet.len += (g_mapUser[(*iter)]->nameLen + 1);
			for (int i = 0; i < g_mapUser[(*iter)]->nameLen; i++)
			{
				packet.szBuf[totalLen++] = g_mapUser[(*iter)]->name[i];
			}
			packet.szBuf[totalLen++] = '\0';
		}
		packet.userNum = g_arrRoom[g_mapUser[ptr]->roomNumber].playerNum;
		packet.header.wLen = sizeof(PACKET_HEADER) + (sizeof(int) *2) + (sizeof(char) * packet.len);

		std::cout << "������ : " <<packet.len << " ->";
		for (int i = 0; i < packet.len; i++)
			std::cout << packet.szBuf[i];
		std::cout << std::endl;

		for (auto iter = g_arrRoom[g_mapUser[ptr]->roomNumber].vecUserSocket.begin();
			iter != g_arrRoom[g_mapUser[ptr]->roomNumber].vecUserSocket.end(); iter++)
		{
			send((*iter)->sock, (const char*)&packet, packet.header.wLen, 0);
		}
	}
	break;
	}

	memcpy(&pUser->szBuf, &pUser->szBuf[header.wLen], pUser->len - header.wLen);
	pUser->len -= header.wLen;

	return true;
}

// ���� �Լ� ���� ��� �� ����
void err_quit(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void sendRoomExit(SOCKETINFO* ptr)
{
	PACKET_SEND_USER_VIEW packet;
	packet.header.wIndex = PACKET_INDEX_SEND_USER_VIEW;
	packet.len = 0;
	int totalLen = 0;
	for (auto iter = g_arrRoom[g_mapUser[ptr]->roomNumber].vecUserSocket.begin();
		iter != g_arrRoom[g_mapUser[ptr]->roomNumber].vecUserSocket.end(); iter++)
	{
		packet.len += (g_mapUser[(*iter)]->nameLen + 1);
		for (int i = 0; i < g_mapUser[(*iter)]->nameLen; i++)
		{
			packet.szBuf[totalLen++] = g_mapUser[(*iter)]->name[i];
		}
		packet.szBuf[totalLen++] = '\0';
	}
	packet.userNum = g_arrRoom[g_mapUser[ptr]->roomNumber].playerNum;
	packet.header.wLen = sizeof(PACKET_HEADER) + (sizeof(int) * 2) + (sizeof(char) * packet.len);

	std::cout << "������ : " << packet.len << " ->";
	for (int i = 0; i < packet.len; i++)
		std::cout << packet.szBuf[i];
	std::cout << std::endl;

	for (auto iter = g_arrRoom[g_mapUser[ptr]->roomNumber].vecUserSocket.begin();
		iter != g_arrRoom[g_mapUser[ptr]->roomNumber].vecUserSocket.end(); iter++)
	{
		send((*iter)->sock, (const char*)&packet, packet.header.wLen, 0);
	}
}