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

void BitMap::Render(int x, int y)
{
	//BitBlt(hdc, x, y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, SRCCOPY);
	TransparentBlt(m_hDC, x, y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 0, 255));
}

void BitMap::RenderCheck(int x, int y)
{
	TCHAR szBuf[128];
	wsprintf(szBuf, TEXT("x :%d \n y :%d"), x, y);
	TextOut(m_hDC, x, y, szBuf, lstrlen(szBuf));
}

void BitMap::RenderRECT(RECT rt)
{
	Rectangle(m_hDC, rt.left + 20, rt.top + 20, rt.right + 20, rt.bottom + 20);
}

void BitMap::Release()
{
	SelectObject(m_hMemDC, m_hOldBitMap);
	DeleteObject(m_hBitMap);
	DeleteDC(m_hMemDC);
}
