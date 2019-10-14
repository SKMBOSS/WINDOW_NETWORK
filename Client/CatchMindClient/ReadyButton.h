#pragma once
#include <Windows.h>

class BitMap;
class ReadyButton
{
private:
	HWND		m_hWnd;
	BitMap*		m_pReadyButtonBitMap;
	POINT		m_pos;

	bool		m_bKeyFirst;
public:
	void Init(HWND hWnd);
	void Update();
	void Render();
public:
	void SendReadyButtonClick();
public:
	ReadyButton();
	~ReadyButton();
};
