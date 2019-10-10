#pragma once
#include "Scene.h"
#include <vector>
using namespace std;

class BackGround;
class Chat;
class Lobby :
	public Scene
{
private:
	HWND			m_hWnd;
	HINSTANCE		m_hInst;

	Chat*			m_pChat;
	BackGround*		m_pBackGround;

public:
	virtual void Init(HWND hWnd, HINSTANCE hInst);
	virtual void Update(float fElapseTime);
	virtual void Render();
	virtual void Release();
	virtual void ProcessPacket(char* szBuf, int len);
public:
	Lobby();
	~Lobby();
};

