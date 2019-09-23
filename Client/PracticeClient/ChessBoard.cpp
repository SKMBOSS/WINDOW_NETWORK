#include "ChessBoard.h"
#include "Macro.h"
#include "Tile.h"

ChessBoard::ChessBoard()
{
}

ChessBoard::~ChessBoard()
{
}

void ChessBoard::Init()
{
	for (int i = 0; i < 8 * 8; i++)
	{
		Tile* tile = new Tile();
		m_vecTile.push_back(tile);
	}
	for (auto iter = m_vecTile.begin(); iter != m_vecTile.end(); ++iter)
		(*iter)->Init();
}

void ChessBoard::Render()
{
	for (auto iter = m_vecTile.begin(); iter != m_vecTile.end(); iter++)
	{
		(*iter)->Render();
	}
}

void ChessBoard::Update(int x, int y)
{
	for (auto iter = m_vecTile.begin(); iter != m_vecTile.end(); iter++)
	{
		(*iter)->Update(x, y);
	}
}

void ChessBoard::Release()
{
	for (auto iter = m_vecTile.begin(); iter != m_vecTile.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecTile.clear();
}