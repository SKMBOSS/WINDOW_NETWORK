#pragma once
#include <Windows.h>
#include <deque>
#include <string>
#define ID_EDIT 100
#define MAX_RENDER_MSG 5
using namespace std;

class Chat
{
	HWND		m_hWnd;
	HINSTANCE   m_hInst;

	HWND			m_hChatWnd;
	string			m_msg;
	string			m_clearMsg;
	deque<string>   m_deqRendrMsg;
	bool		    m_bKeyFirst;

public:
	void Init(HWND hWnd, HINSTANCE hInst);
	void Update();
	void Render();
	void Relaese();
public:
	void ProcessPacket(char* szBuf, int len);
	void SendChatMsg(char *msg, int len);

public:
	Chat();
	~Chat();
};

