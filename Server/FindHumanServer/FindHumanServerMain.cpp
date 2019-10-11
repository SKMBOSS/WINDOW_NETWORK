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

// 소켓 정보 저장을 위한 구조체
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

// 작업자 스레드 함수
DWORD WINAPI WorkerThread(LPVOID arg);
//test func
bool ProcessPacket(SOCKETINFO* ptr, USER_INFO* pUser, DWORD &len);
// 오류 출력 함수
void err_quit(const char *msg);
void err_display(const char *msg);

void sendRoomExit(SOCKETINFO* ptr);

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	// 입출력 완료 포트 생성
	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL) return 1;

	// CPU 개수 확인
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	// (CPU 개수 * 2)개의 작업자 스레드 생성
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

	// 데이터 통신에 사용할 변수
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
		printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 소켓과 입출력 완료 포트 연결
		CreateIoCompletionPort((HANDLE)client_sock, hcp, client_sock, 0);

		// 소켓 정보 구조체 할당
		SOCKETINFO *ptr = new SOCKETINFO;
		if (ptr == NULL) break;
		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
		ptr->sock = client_sock;
		ptr->recvbytes = ptr->sendbytes = 0;
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = BUFSIZE;

		//로그인패킷 시작
		USER_INFO* pInfo = new USER_INFO();
		pInfo->index = g_iIndex++;
		pInfo->len = 0;
		g_mapUser.insert(std::make_pair(ptr, pInfo));

		// 비동기 입출력 시작
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

	// 윈속 종료
	WSACleanup();
	return 0;
}

//작업자 스레드 함수
DWORD WINAPI WorkerThread(LPVOID arg)
{
	int retval;
	HANDLE hcp = (HANDLE)arg;

	while (1)
	{
		//std::cout << std::this_thread::get_id() << std::endl;
		//비동기 입출력 완료 기다리기
		DWORD cbTransferred;
		SOCKET client_sock;
		SOCKETINFO *ptr;
		retval = GetQueuedCompletionStatus(hcp, &cbTransferred,
			(PULONG_PTR)&client_sock, (LPOVERLAPPED *)&ptr, INFINITE);

		//클라이언트 정보 얻기
		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);

		//비동기 입출력 결과 확인
		if (retval == 0 || cbTransferred == 0)
		{
			if (retval == 0) {
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &ptr->overlapped,
					&temp1, FALSE, &temp2);
				err_display("WSAGetOverlappedResult()");
			}
			//방에 있는 유저정보 삭제하고
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
			//방정보 보내주고
			sendRoomExit(ptr);
			//서버 유저 정보 삭제하고
			g_mapUser.erase(ptr);
			//소켓닫고
			closesocket(ptr->sock);
			printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
			delete ptr;
			continue;
		}

		// 데이터 받기
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

		g_mapUser[ptr]->roomNumber = 0;//로비 입장

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

		std::cout << "보냇다 : " <<packet.len << " ->";
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

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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

	std::cout << "보냇다 : " << packet.len << " ->";
	for (int i = 0; i < packet.len; i++)
		std::cout << packet.szBuf[i];
	std::cout << std::endl;

	for (auto iter = g_arrRoom[g_mapUser[ptr]->roomNumber].vecUserSocket.begin();
		iter != g_arrRoom[g_mapUser[ptr]->roomNumber].vecUserSocket.end(); iter++)
	{
		send((*iter)->sock, (const char*)&packet, packet.header.wLen, 0);
	}
}