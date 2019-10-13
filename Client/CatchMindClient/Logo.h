#pragma once
#include <Windows.h>

class BitMap;
class Logo
{
private:
	BitMap*		m_LogoBitMap;
	POINT		m_pos;
public:
	void Init();
	void Render();

public:
	Logo();
	~Logo();
};

