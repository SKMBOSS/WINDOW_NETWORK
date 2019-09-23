#include "Tile.h"
#include "BitMap.h"
#include "ResourceManager.h"

int Tile::m_sTileNumber = 0;

Tile::Tile()
{
	m_iThisNumber = m_sTileNumber++;

	if ((m_iThisNumber >= 0 && m_iThisNumber < 8)
		|| (m_iThisNumber >= 16 && m_iThisNumber < 24)
		|| (m_iThisNumber >= 32 && m_iThisNumber < 40)
		|| (m_iThisNumber >= 48 && m_iThisNumber < 56))
	{
		if (m_iThisNumber % 2 == 0)
			m_pBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BOARD_WHITE);
		else
			m_pBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BOARD_BLACK);
	}
	else
	{
		if (m_iThisNumber % 2 == 0)
			m_pBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BOARD_BLACK);
		else
			m_pBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BOARD_WHITE);
	}
	m_selectedTile = ResourceManager::GetInstance()->GetBitMap(RES_BOARD_SELECTED);
	m_bIsSelectd = false;
}

Tile::~Tile()
{
}


void Tile::Init()
{
	m_pos.x = ((m_iThisNumber % 8) * m_pBitMap->GetSize().cx);
	m_pos.y = ((m_iThisNumber / 8) * m_pBitMap->GetSize().cy);
}

void Tile::Update(int x, int y)
{
	RECT rc = { m_pos.x , m_pos.y, m_pos.x + m_pBitMap->GetSize().cx, m_pos.y + m_pBitMap->GetSize().cy };
	POINT pt = { x, y };

	if (PtInRect(&rc, pt))
	{
		m_bIsSelectd = true;
	}
}

void Tile::Render()
{
	m_pBitMap->Render(m_pos.x, m_pos.y);
	if (m_bIsSelectd)
		m_selectedTile->Render(m_pos.x, m_pos.y);
}