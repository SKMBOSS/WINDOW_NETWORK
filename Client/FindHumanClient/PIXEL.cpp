#include "PIXEL.h"

PIXEL::PIXEL()
{
}

PIXEL::~PIXEL()
{
}

void PIXEL::SetPixel(POINT newPos, POINT oldPos, int pixelWidth, COLORREF color)
{
	m_newPos = newPos;
	m_oldPos = oldPos;
	m_pixelWidth = pixelWidth;
	m_color = color;
}
