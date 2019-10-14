#pragma once
#include <Windows.h>

class BitMap;
class StartButton
{
private:
	HWND		m_hWnd;
	BitMap*		m_pStartButtonBitMap;
	POINT		m_pos;

	bool		m_bKeyFirst;
public:
	void Init(HWND hWnd);
	void Update();
	void Render();
public:
	void SendStartButtonClick();
public:
	StartButton();
	~StartButton();
};
