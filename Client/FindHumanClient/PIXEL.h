#pragma once
#include <Windows.h>

class BitMap;
class PIXEL
{
private:	
	BitMap*		m_pPixelBitmap;
	POINT		m_pPos;

public:
	void Init(int x, int y);
	void Render();
public:
	PIXEL();
	~PIXEL();
};

