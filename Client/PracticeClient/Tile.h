#pragma once
#include <Windows.h>

class BitMap;
class Tile
{
private:
	static int	m_sTileNumber;
	int			m_iThisNumber;
	POINT		m_pos;
	BitMap*		m_pBitMap;

	BitMap*		m_selectedTile;
	bool		m_bIsSelectd;
public:
	void Init();
	void Render();
	void Update(int x, int y);
	
	Tile();
	~Tile();
};
