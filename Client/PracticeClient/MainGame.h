#pragma once
#include "Scene.h"
#include "string"
#define ID_EDIT 100

using namespace std;

class ChessBoard;
class Chat;
class MainGame :
	public Scene
{
private:
	HWND		m_hWnd;
	HINSTANCE   m_hInst;
	ChessBoard* m_chessBoard;
	Chat*		m_chat;
	bool		m_bReady = false;
public:
	virtual void Init(HWND hWnd, HINSTANCE hInst);
	virtual void Update(float fElapseTime);
	virtual void Render();
	virtual void Release();
	virtual void ProcessPacket(char* szBuf, int len);
public:
	void SendTurn();
	void SendReady();
public:
	MainGame();
	~MainGame();
};

