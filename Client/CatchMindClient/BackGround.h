#pragma once
#include <Windows.h>

class BitMap;
class BackGround
{
private:
	BitMap*		m_pBackGroundBitMap;
	POINT		m_pos;
public:
	void Init();
	void Render();
public:
	BackGround();
	~BackGround();
};

