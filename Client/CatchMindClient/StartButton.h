#pragma once
#include <Windows.h>

class BitMap;
class StartButton
{
private:
	BitMap*		m_pStartButtonBitMap;
	POINT		m_pos;
public:
	void Init();
	void Render();
public:
	StartButton();
	~StartButton();
};
