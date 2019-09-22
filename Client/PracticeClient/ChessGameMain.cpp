#pragma comment(lib, "ws2_32")
#pragma comment(lib, "msimg32")
#include <WinSock2.h>
#include <windows.h>
#include <map>
#include "..\..\Common\PACKET_HEADER.h"
#include "ChessGame.h"
#include "NetworkManager.h"
using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ProcessPacket(char* szBuf, int len);
HINSTANCE g_hInst;
HWND hWnd;
char g_szClassName[256] = "ChessGame";
ChessGame g_ChessGame;

#define BUFSIZE 512
#define WM_SOCKET (WM_USER+1)

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = g_szClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(g_szClassName, g_szClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	NetworkManager::GetInstance()->SetSocket(socket(AF_INET, SOCK_STREAM, 0));
	//NetworkManager::GetInstance()->GetSocket() = socket(AF_INET, SOCK_STREAM, 0);
	if (NetworkManager::GetInstance()->GetSocket() == INVALID_SOCKET)
	{
		//cout << "err on socket" << endl;
		return -1;
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int retval = connect(NetworkManager::GetInstance()->GetSocket(), (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		//cout << "err on connect" << endl;
		return -1;
	}

	retval = WSAAsyncSelect(NetworkManager::GetInstance()->GetSocket(), hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	if (retval == SOCKET_ERROR)
	{
		return -1;
	}

	/*

	g_ChessGame.Init(hWnd);

	while (true)
	{
		/// 메시지큐에 메시지가 있으면 메시지 처리
		if (PeekMessage(&Message, NULL, 0U, 0U, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
				break;

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			g_ChessGame.Update();
			g_ChessGame.Render();
		}
	}*/

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	g_ChessGame.Release();
	NetworkManager::GetInstance()->GetSocket();
	NetworkManager::GetInstance()->Release();
	WSACleanup();

	return (int)Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_CREATE:
		return 0;
	case WM_SOCKET:
		ProcessSocketMessage(hWnd, iMessage, wParam, lParam);
		InvalidateRect(hWnd, NULL, true);
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			g_mapPlayer[g_iIndex]->x -= 8;
			SendPos();
			break;
		case VK_RIGHT:
			g_mapPlayer[g_iIndex]->x += 8;
			SendPos();
			break;
		case VK_UP:
			g_mapPlayer[g_iIndex]->y -= 8;
			SendPos();
			break;
		case VK_DOWN:
			g_mapPlayer[g_iIndex]->y += 8;
			SendPos();
			break;
		}

		InvalidateRect(hWnd, NULL, true);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (auto iter = g_mapPlayer.begin(); iter != g_mapPlayer.end(); iter++)
		{
			char szPrint[128];
			wsprintf(szPrint, "%d", iter->first);
			TextOut(hdc, iter->second->x, iter->second->y, szPrint, strlen(szPrint));

		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		for (auto iter = g_mapPlayer.begin(); iter != g_mapPlayer.end(); iter++)
		{
			delete iter->second;
		}
		g_mapPlayer.clear();
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
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
		//err_display(WSAGETSELECTERROR(lParam));
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
	{
		char szBuf[BUFSIZE];

		retval = recv(wParam, szBuf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				//cout << "err on recv!!" << endl;
			}
		}

		ProcessPacket(szBuf, retval);
	}
	break;
	case FD_CLOSE:
		closesocket(wParam);
		break;
	}
}

void ProcessPacket(char* szBuf, int len)
{
	PACKET_HEADER header;

	memcpy(&header, szBuf, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_LOGIN_RET:
	{
		PACKET_LOGIN_RET packet;
		memcpy(&packet, szBuf, header.wLen);

		g_iIndex = packet.iIndex;
	}
	break;
	case PACKET_INDEX_USER_DATA:
	{
		PACKET_USER_DATA packet;
		memcpy(&packet, szBuf, header.wLen);

		for (auto iter = g_mapPlayer.begin(); iter != g_mapPlayer.end(); iter++)
		{
			delete iter->second;
		}
		g_mapPlayer.clear();

		for (int i = 0; i < packet.wCount; i++)
		{
			Player* pNew = new Player();
			pNew->x = packet.data[i].wX;
			pNew->y = packet.data[i].wY;
			g_mapPlayer.insert(make_pair(packet.data[i].iIndex, pNew));
		}
	}
	break;
	case PACKET_INDEX_SEND_POS:
	{
		PACKET_SEND_POS packet;
		memcpy(&packet, szBuf, header.wLen);

		g_mapPlayer[packet.data.iIndex]->x = packet.data.wX;
		g_mapPlayer[packet.data.iIndex]->y = packet.data.wY;
	}
	break;
	}
}
