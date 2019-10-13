#pragma once
#include "Scene.h"
#include "string"
#define ID_EDIT 100

class BackGround;
class Chat;
class UserView;
class PaintBoard;
class MainGame :
	public Scene
{
private:
	HWND				m_hWnd;
	HINSTANCE			m_hInst;

	BackGround*			m_pBackGround;
	UserView*			m_pUserView;
	Chat*				m_pChat;

	PaintBoard*			m_pPaintBoard;
public:
	virtual void Init(HWND hWnd, HINSTANCE hInst);
	virtual void Update(float fElapseTime);
	virtual void Render();
	virtual void Release();
	virtual void ProcessPacket(char* szBuf, int len);
public:
	void SendUserViewUpdate();
public:
	MainGame();
	~MainGame();
};

