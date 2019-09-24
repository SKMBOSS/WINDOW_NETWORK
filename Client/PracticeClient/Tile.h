#pragma once
#include <Windows.h>

enum TILE_STATE
{
	BLANK,

	W_PAWN,
	W_KNIGHT,
	W_BISHOP,
	W_ROOK,
	W_QUEEN,
	W_KING,

	B_PAWN,
	B_KNIGHT,
	B_BISHOP,
	B_ROOK,
	B_QUEEN,
	B_KING,
};

class BitMap;
class Tile
{
private:
	static int	m_sTileNumber;
	int			m_iThisNumber;
	POINT		m_pos;
	BitMap*		m_pTileBitMap;

	BitMap*		m_selectedTile;
	bool		m_bIsSelectd;

	BitMap*		m_pChessPieceBitMap;
	TILE_STATE	m_eTileState;

public:
	void Init();
	void Render();
	void Update(int x, int y);
public:
	void InitTileInfo();
	void InitChessPiece();
	bool IsSelectedTile(int x, int y);
public:
	inline TILE_STATE GetTileState()
	{
		return m_eTileState;
	}
	inline int GetTileNumber()
	{
		return m_iThisNumber;
	}
	inline void SetIsSelected()
	{
		m_bIsSelectd = true;
	}
	inline void SetIsNoSelected()
	{
		m_bIsSelectd = false;
	}
	inline bool GetIsSelected()
	{
		return m_bIsSelectd;
	}
	void SetTileState(TILE_STATE state);
public:
	Tile();
	~Tile();
};
