#pragma once
#include <Windows.h>

class PIXEL
{
public:
	POINT			m_newPos;
	POINT			m_oldPos;
	int				m_pixelWidth;
	COLORREF		m_color;

public:
	void SetPixel(POINT newPos, POINT oldPos, int pixelWidth, COLORREF color);
public:
	PIXEL();
	~PIXEL();
};

