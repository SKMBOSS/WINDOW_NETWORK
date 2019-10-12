#pragma once
#include <Windows.h>
#include <vector>
using namespace std;

class BitMap;
class PIXEL;
class PaintBoard
{
private:
	BitMap*				m_pPaintBoard;
	POINT				m_Pos;

	vector<PIXEL*>		m_vecPIXEL;
public:
	void Init();
	void Update(int x, int y);
	void Render();
	void Release();
public:
	PaintBoard();
	~PaintBoard();
};

