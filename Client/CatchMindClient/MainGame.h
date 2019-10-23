#pragma once
#include "Scene.h"
#include "string"
#define ID_EDIT 100

class BackGround;
class ReadyButton;
class StartButton;
class UserView;
class Chat;
class PaintBoard;
class UserReadyBox;
class MainGame :
	public Scene
{
private:
	HWND				m_hWnd;
	HINSTANCE			m_hInst;

	BackGround*			m_pBackGround;

	ReadyButton*		m_pReadyButton;
	bool				m_bReadyButtonActive;

	StartButton*		m_pStartButton;
	bool				m_bStartButtonActive;

	UserView*			m_pUserView;
	UserReadyBox*		m_pUserReadyBox;

	Chat*				m_pChat;
	bool				m_bChatting;

	PaintBoard*			m_pPaintBoard;
	bool				m_bPaingting;


public:
	virtual void Init(HWND hWnd, HINSTANCE hInst);
	virtual void Update(float fElapseTime);
	virtual void Render();
	virtual void Release();
	virtual void ProcessPacket(char* szBuf, int len);
public:
	void SendUserViewUpdate();
	void SendUserReadyBoxUpdate();
public:
	MainGame();
	virtual ~MainGame();
};

