#include "BitMap.h"

BitMap::BitMap()
{
}

BitMap::~BitMap()
{
}

void BitMap::Init(HDC hdc, string path)
{
	m_hDC = hdc;
	m_hMemDC = CreateCompatibleDC(hdc);
	m_hBitMap = (HBITMAP)LoadImage(NULL, path.c_str(), IMAGE_BITMAP
		, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_hOldBitMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);

	BITMAP bitmap;
	GetObject(m_hBitMap, sizeof(bitmap), &bitmap);
	m_size.cx = bitmap.bmWidth;
	m_size.cy = bitmap.bmHeight;
}

void BitMap::Render(int pX, int pY, int x, int y)
{
	TransparentBlt(m_hDC, x, y, pX, pY, m_hMemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 0, 255));
}

void BitMap::RenderCheck(int x, int y)
{
	TCHAR szBuf[128];
	wsprintf(szBuf, TEXT("x :%d \n y :%d"), x, y);
	TextOut(m_hDC, x, y, szBuf, lstrlen(szBuf));
}

void BitMap::RenderText(int x, int y, string text)
{
	/*TCHAR szBuf[128];
	wsprintf(szBuf, TEXT("x :%d \n y :%d"), x, y);*/
	SetBkMode(m_hDC, TRANSPARENT);
	TextOut(m_hDC, x, y, text.c_str(), text.length());
}

void BitMap::RenderAlphaBlend(int pX, int pY, int x, int y, int alpha)
{
	BLENDFUNCTION bf;
	ZeroMemory(&bf, sizeof(BLENDFUNCTION));  //멤버들을 모두 0으로...
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = alpha; //0~255 값을 가지는 투명도

	AlphaBlend(m_hDC, x, y, pX, pY, m_hMemDC, 0, 0, m_size.cx, m_size.cy, bf);
}

void BitMap::Release()
{
	SelectObject(m_hMemDC, m_hOldBitMap);
	DeleteObject(m_hBitMap);
	DeleteDC(m_hMemDC);
}
