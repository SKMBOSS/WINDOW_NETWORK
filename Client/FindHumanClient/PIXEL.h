#pragma once
#include <Windows.h>

class BitMap;
class PIXEL
{
public:	
	BitMap*		m_pPixelBitmap;
	POINT		m_Pos;
public:
	void Init(int x, int y);
	void Render();
public:
	PIXEL();
	~PIXEL();
};

