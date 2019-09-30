#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <map>
#include "..\..\Common\PACKET_HEADER.h"
using namespace std;

#define BUFSIZE 512
#define WM_SOCKET (WM_USER+1)

class USER_INFO
{
public:
	int index;
	char szBuf[BUFSIZE];
	int len;
	bool myTurn;
	int roomNumber;
};

class ROOM_INFO
{
public:
	int userNumCount;
};


int g_iIndex = 0;
map<SOCKET, USER_INFO*> g_mapUser;
ROOM_INFO g_arrRoom[16];


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ProcessSocketMessage(HWND, UINT, WPARAM, LPARAM);
bool ProcessPacket(SOCKET sock, USER_INFO* pUser, char* szBuf, int& len);
void err_display(int errcode);
void err_display(const char* szMsg);

int main(int argc, char* argv[])
{
	//이러면 안되요
	for (int i = 0; i < 16; i++)
	{
		g_arrRoom[i].userNumCount = 0;
	}
	//알고있어요

	int retval;

	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = NULL;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = "WSAAsyncSelectServer";
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClass(&WndClass))
		return -1;

	HWND hWnd = CreateWindow("WSAAsyncSelectServer", "TCP 서버", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, NULL, NULL);
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		cout << "err on socket" << endl;
		return -1;
	}

	retval = WSAAsyncSelect(listen_sock, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
	if (retval == SOCKET_ERROR)
	{
		cout << "err on WSAAsyncSelect" << endl;
		return -1;
	}

	//bind
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		cout << "err on bind" << endl;
		return -1;
	}

	//listen
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		cout << "err on listen" << endl;
		return -1;
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SOCKET:
		ProcessSocketMessage(hWnd, uMsg, wParam, lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen = 0;
	int retval = 0;


	if (WSAGETSELECTERROR(lParam))
	{
		int err_code = WSAGETSELECTERROR(lParam);
		err_display(WSAGETSELECTERROR(lParam));
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(wParam, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			int err_code = WSAGetLastError();
			if (err_code != WSAEWOULDBLOCK)
			{
				err_display("err on accept");
			}
			return;
		}
		printf("[TCP 서버] 클라이언트 접속 : IP 주소 = %s , 포트번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		retval = WSAAsyncSelect(client_sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
		if (retval == SOCKET_ERROR)
		{
			cout << "err on WSAAsyncSelect!!" << endl;
		}

		USER_INFO* pInfo = new USER_INFO();
		pInfo->index = g_iIndex++;
		pInfo->len = 0;
		//룸넘버
		/*pInfo->roomNumber = pInfo->index / 2;*/
		pInfo->roomNumber = 99;  // 99 : 로비
		if (pInfo->index % 2 == 0)
			pInfo->myTurn = true;
		else
			pInfo->myTurn = false;
		g_mapUser.insert(make_pair(client_sock, pInfo));

		PACKET_LOGIN_RET packet;
		packet.header.wIndex = PACKET_INDEX_LOGIN_RET;
		packet.header.wLen = sizeof(packet);
		packet.iIndex = pInfo->index;
		send(client_sock, (const char*)&packet, packet.header.wLen, 0);

		Sleep(500);

		PACKET_USER_DATA user_packet;
		user_packet.header.wIndex = PACKET_INDEX_USER_DATA;
		user_packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(WORD) + sizeof(USER_DATA) * g_mapUser.size();
		user_packet.wCount = g_mapUser.size();
		int i = 0;
		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++, i++)
		{	
			user_packet.data[i].iIndex = iter->second->index;
			user_packet.data[i].roomNumber= iter->second->roomNumber;
			user_packet.data[i].turn = iter->second->myTurn;
		}

		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++, i++)
		{
			send(iter->first, (const char*)&user_packet, user_packet.header.wLen, 0);
		}
	}
	break;
	case FD_READ:
	{
		char szBuf[BUFSIZE];

		retval = recv(wParam, szBuf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				cout << "err on recv!!" << endl;
			}
		}

		USER_INFO* pUser = g_mapUser[wParam];

		while (true)
		{
			if (!ProcessPacket(wParam, pUser, szBuf, retval))
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

	}
	break;
	case FD_CLOSE:
		
		cout << g_mapUser[wParam]->index<< "번 GUEST 나감" << endl;
		g_mapUser.erase(wParam);
		closesocket(wParam);
		

		break;
	}
}

bool ProcessPacket(SOCKET sock, USER_INFO* pUser, char* szBuf, int& len)
{
	if (len > 0)
	{
		memcpy(&pUser->szBuf[pUser->len], szBuf, len);
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
	case PACKET_INDEX_SEND_TURN_END:
	{
		PACKET_SEND_TURNEND packet;
		memcpy(&packet, szBuf, header.wLen);

		cout << packet.data.roomNumber << "번 방에서 이동요청" << endl;

		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			//if (iter->first == sock)
				//continue;
			if (iter->second->roomNumber == packet.data.roomNumber)
			{
				if (iter->second->myTurn == true)
					iter->second->myTurn = false;
				else
					iter->second->myTurn = true;

				packet.data.iIndex = iter->second->index;
				packet.data.turn = iter->second->myTurn;
				send(iter->first, (const char*)&packet, header.wLen, 0);
				cout << packet.data.roomNumber << "번 방에 이동승인" << endl;
			}
		}
	}
	break;

	case PACKET_INDEX_SEND_CHAT:
	{
		PACKET_SEND_CHAT packet;
		memcpy(&packet, szBuf, header.wLen);

		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			if (iter->second->roomNumber == packet.roomNumber)
			send(iter->first, (const char*)&packet, header.wLen, 0);
		}
	}
	break;
	case PACKET_INDEX_SEND_CAHNGE_ROOM:
	{
		PACKET_SEND_CAHNGE_ROOM packet;
		memcpy(&packet, szBuf, header.wLen);

		g_mapUser[sock]->roomNumber = packet.roomNumber;
		//test
		g_arrRoom[packet.roomNumber].userNumCount++;
		//test
		cout << g_mapUser[sock]->index << "번 GUEST가 " << packet.roomNumber << "방 클릭함" << endl;

		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			send(iter->first, (const char*)&packet, header.wLen, 0);
		}
	}
	break;
	case PACKET_INDEX_SEND_UPDATE_ROOM:
	{
		PACKET_SEND_UPDATE_ROOM packet;
		memcpy(&packet, szBuf, header.wLen);

		for (int i = 0; i < 16; i++)
			packet.userNumCount[i] = g_arrRoom[i].userNumCount;

		for (int i = 0; i < 16; i++)
		{
			cout << "[" << g_arrRoom[i].userNumCount << "] ";
			if (i == 7 || i == 15)
				cout << endl;
		}

		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			if (iter->second->roomNumber == 99)
			{
				send(iter->first, (const char*)&packet, header.wLen, 0);
				cout << "보냄" << endl;
			}
				
		}
	}
	break;

	case PACKET_INDEX_SEND_TURN_INIT:
	{
		PACKET_SEND_TURN_INIT packet;
		memcpy(&packet, szBuf, header.wLen);

		if (g_arrRoom[packet.data.roomNumber].userNumCount == 1)
		{
			g_mapUser[sock]->myTurn = true;
			packet.color = 0; //BALCK;
		}
			
		else if (g_arrRoom[packet.data.roomNumber].userNumCount == 2)
		{
			g_mapUser[sock]->myTurn = false;
			packet.color = 1; //WHITE;
		}
			
		packet.data.turn = g_mapUser[sock]->myTurn;
		send(sock, (const char*)&packet, header.wLen, 0);
		
	}
	break;

	case PACKET_INDEX_SEND_READY:
	{
		PACKET_SEND_READY packet;
		memcpy(&packet, szBuf, header.wLen);

		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			if (iter->second->roomNumber == packet.roomNumber)
			{
				send(iter->first, (const char*)&packet, header.wLen, 0);
			}
		}
	}
	break;
	}

	memcpy(&pUser->szBuf, &pUser->szBuf[header.wLen], pUser->len - header.wLen);
	pUser->len -= header.wLen;

	return true;
}

void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[오류]%s", lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void err_display(const char* szMsg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s]%s\n", szMsg, lpMsgBuf);
	LocalFree(lpMsgBuf);
}