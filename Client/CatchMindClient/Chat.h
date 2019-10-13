#pragma once
#include <Windows.h>
#include <deque>
#include <string>
#define ID_EDIT 100

using namespace std;

class Chat
{
	HWND		m_hWnd;
	HINSTANCE   m_hInst;

	HWND			m_hChatWnd;
	string			m_clearMsg;
	deque<string>   m_deqRendrMsg;
	bool		    m_bKeyFirst;
	int				m_maxRenderText;

public:
	void Init(HWND hWnd, HINSTANCE hInst, int maxRenderText);
	void Update();
	void Render();
	void Relaese();
public:
	void ProcessPacket(char* szBuf, int len);
	void SendChatMsg(char *msg, int len);
	void LimitText(int limitIndex);

public:
	Chat();
	~Chat();
};

