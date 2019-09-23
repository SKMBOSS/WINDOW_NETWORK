#pragma once
#include "Scene.h"

using namespace std;

class ChessBoard;
class MainGame :
	public Scene
{
private:
	HWND		m_hWnd;
	ChessBoard* m_chessBoard;
public:
	virtual void Init(HWND hWnd);
	virtual void Update(float fElapseTime);
	virtual void Render();
	virtual void Release();
	virtual void ProcessPacket(char* szBuf, int len);

	void SendPos();
public:
	MainGame();
	~MainGame();
};

