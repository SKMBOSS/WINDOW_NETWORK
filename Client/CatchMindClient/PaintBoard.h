#pragma once
#include <Windows.h>
#include <vector>
using namespace std;

enum PAINT_BOARD_STATE
{
	PB_WAITING,
	PB_PAINTING,
	PB_PAINTING_END,
};

class BitMap;
class PIXEL;
class PaintBoard
{
private:
	HDC					m_hdc;

	BitMap*				m_pPaintBoard;
	POINT				m_PaintBoardPos;

	BitMap*				m_pPalette;
	POINT				m_paletePos;

	BitMap*				m_pPixcel;
	POINT				m_pixcelPos;

	PAINT_BOARD_STATE   m_ePaintBoardState;
	HPEN				m_hPen;
	HPEN				m_hOldPen;

	vector<PIXEL*>		m_vecPIXEL;
	POINT				m_oldPixelPos;
	int					m_pixelWidth;
	COLORREF			m_color;

	bool				m_bKeyFirst;

public:
	void Init();
	void Update(HWND hWnd);
	void Render();
	void Release();
	void ProcessPacket(char* szBuf, int len);
public:
	void SendPixelPos(POINT newPos, POINT oldPos, int pixelWidth, COLORREF color);
	void SendAllDelete();
	void MakeAndPushPixel(POINT newPos, POINT oldPos, int pixelWidth, COLORREF color);
public:
	PaintBoard();
	~PaintBoard();
};

