#pragma once
#include <Windows.h>
#include <chrono>
using namespace std;

class ChessGame
{
private:
	HWND		m_hWnd;
	HDC			m_hMemDC;
	HBITMAP		m_hBitmap;
	HBITMAP		m_hOld;
	chrono::system_clock::time_point m_LastTime;
	float		m_fElapseTime;//경과시간
public:
	void Init(HWND hWnd);
	void Update();
	void Render();
	void Release();
public:
	ChessGame();
	~ChessGame();
};

