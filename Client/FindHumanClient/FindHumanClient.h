#pragma once
#include <Windows.h>
#include <chrono>
using namespace std;

class FindHumanClient
{
private:
	static FindHumanClient* m_sThis;
private:
	HWND		m_hWnd;
	HINSTANCE   m_hInst;
	chrono::system_clock::time_point m_LastTime;
	float		m_fElapseTime;//경과시간

	HDC			m_hMemDC;
	HBITMAP		m_hBitmap;
	HBITMAP		m_hOld;

	int			myLen;
	char		packetBuf[1024];
public:
	static FindHumanClient* GetInstance();
public:
	void Init(HWND hWnd, HINSTANCE hInst, SOCKET sock);
	void Update();
	void Render();
	void Release();
	void ProcessPacket(char* szBuf, int len);
	void ProcessPacketBuf();
public:
	void SendLoginOk();
private:
	FindHumanClient();
public:
	~FindHumanClient();
};

