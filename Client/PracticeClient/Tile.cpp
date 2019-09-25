#include "Tile.h"
#include "BitMap.h"
#include "ResourceManager.h"

int Tile::m_sTileNumber = 0;

Tile::Tile()
{
}

Tile::~Tile()
{
}

void Tile::Init()
{
	InitTileInfo();
	InitChessPiece();
}

void Tile::Update(int x, int y)
{
	//
}
	

void Tile::Render()
{
	m_pTileBitMap->Render(m_pos.x, m_pos.y);

	if(m_eTileState !=BLANK)
		m_pChessPieceBitMap->Render(m_pos.x, m_pos.y);

	if (m_bIsSelectd)
		m_selectedTile->Render(m_pos.x, m_pos.y);
}

bool Tile::IsSelectedTile(int x, int y)
{
	RECT rc = { m_pos.x , m_pos.y, m_pos.x + m_pTileBitMap->GetSize().cx, m_pos.y + m_pTileBitMap->GetSize().cy };
	POINT pt = { x, y };

	if (PtInRect(&rc, pt))
	{
		return true;
	}
	return false;
}

void Tile::InitTileInfo()
{
	m_iThisNumber = m_sTileNumber++;
	m_eTileState = BLANK;
	m_pChessPieceBitMap = nullptr;

	if ((m_iThisNumber >= 0 && m_iThisNumber < 8)
		|| (m_iThisNumber >= 16 && m_iThisNumber < 24)
		|| (m_iThisNumber >= 32 && m_iThisNumber < 40)
		|| (m_iThisNumber >= 48 && m_iThisNumber < 56))
	{
		if (m_iThisNumber % 2 == 0)
			m_pTileBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BOARD_WHITE);
		else
			m_pTileBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BOARD_BLACK);
	}
	else
	{
		if (m_iThisNumber % 2 == 0)
			m_pTileBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BOARD_BLACK);
		else
			m_pTileBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BOARD_WHITE);
	}

	m_pos.x = ((m_iThisNumber % 8) * m_pTileBitMap->GetSize().cx);
	m_pos.y = ((m_iThisNumber / 8) * m_pTileBitMap->GetSize().cy);

	m_selectedTile = ResourceManager::GetInstance()->GetBitMap(RES_BOARD_SELECTED);
	m_bIsSelectd = false;
}

void Tile::InitChessPiece()
{
	if (m_iThisNumber >= 8 && m_iThisNumber <= 15)
	{
		m_eTileState = W_PAWN;
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_WHITE_PAWN);
	}

	else if (m_iThisNumber >= 48 && m_iThisNumber <= 55)
	{
		m_eTileState = B_PAWN;
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BLACK_PAWN);
	}

	else if (m_iThisNumber == 0 || m_iThisNumber == 7)
	{
		m_eTileState = W_ROOK;
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_WHITE_ROOK);
	}

	else if (m_iThisNumber == 56 || m_iThisNumber == 63)
	{
		m_eTileState = B_ROOK;
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BLACK_ROOK);
	}

	else if (m_iThisNumber == 1 || m_iThisNumber == 6)
	{
		m_eTileState = W_KNIGHT;
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_WHITE_KNIGHT);
	}

	else if (m_iThisNumber == 57 || m_iThisNumber == 62)
	{
		m_eTileState = B_KNIGHT;
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BLACK_KNIGHT);
	}

	else if (m_iThisNumber == 2 || m_iThisNumber == 5)
	{
		m_eTileState = W_BISHOP;
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_WHITE_BISHOP);
	}

	else if (m_iThisNumber == 58 || m_iThisNumber == 61)
	{
		m_eTileState = B_BISHOP;
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BLACK_BISHOP);
	}

	else if (m_iThisNumber == 3)
	{
		m_eTileState = W_QUEEN;
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_WHITE_QUEEN);
	}

	else if (m_iThisNumber == 59)
	{
		m_eTileState = B_QUEEN;
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BLACK_QUEEN);
	}

	else if (m_iThisNumber == 4)
	{
		m_eTileState = W_KING;
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_WHITE_KING);
	}

	else if (m_iThisNumber == 60)
	{
		m_eTileState = B_KING;
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BLACK_KING);
	}

}

void Tile::SetTileState(TILE_STATE state)
{
	m_eTileState = state;

	if (m_eTileState == BLANK)
		m_pChessPieceBitMap = nullptr;

	else if (m_eTileState == W_PAWN)
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_WHITE_PAWN);
	else if (m_eTileState == W_KNIGHT)
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_WHITE_KNIGHT);
	else if (m_eTileState == W_BISHOP)
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_WHITE_BISHOP);
	else if (m_eTileState == W_ROOK)
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_WHITE_ROOK);
	else if (m_eTileState == W_QUEEN)
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_WHITE_QUEEN);
	else if (m_eTileState == W_KING)
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_WHITE_KING);

	else if (m_eTileState == B_PAWN)
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BLACK_PAWN);
	else if (m_eTileState == B_KNIGHT)
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BLACK_KNIGHT);
	else if (m_eTileState == B_BISHOP)
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BLACK_BISHOP);
	else if (m_eTileState == B_ROOK)
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BLACK_ROOK);
	else if (m_eTileState == B_QUEEN)
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BLACK_QUEEN);
	else if (m_eTileState == B_KING)
		m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BLACK_KING);

}