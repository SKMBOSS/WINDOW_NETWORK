#pragma once
#include <Windows.h>
#include <string>
using namespace std;

class BitMap
{
private:
	HDC			m_hDC;
	HDC			m_hMemDC;
	HBITMAP		m_hBitMap;
	HBITMAP		m_hOldBitMap;
	SIZE		m_size;
public:
	inline SIZE GetSize()
	{
		return m_size;
	}
public:
	void Init(HDC hdc, string path);
	void Render(int x, int y);
	void RenderCheck(int x, int y);
	void RenderRECT(RECT rt);
	//드로우추가하자
	void Release();
public:
	BitMap();
	~BitMap();
};

