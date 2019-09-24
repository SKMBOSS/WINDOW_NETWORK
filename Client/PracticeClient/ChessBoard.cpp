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
	m_bTurnEnd = false;

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
	if (!m_bTurnEnd)
	{
		SelectOtherTile(x, y);
		MoveChessPiece(x, y);
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

void ChessBoard::ProcessPacket(char* szBuf, int len)
{
//
}

int ChessBoard::GetSelectedTile(int x, int y)
{
	for (auto iter = m_vecTile.begin(); iter != m_vecTile.end(); iter++)
	{
		if ((*iter)->IsSelectedTile(x, y))
		{
			return (*iter)->GetTileNumber();
		}
	}
	return -1;
}

void ChessBoard::ShowMoveableTile(int tileNumber)
{
	TILE_STATE selectedTileState = m_vecTile.at(tileNumber)->GetTileState();

	if (selectedTileState == W_PAWN)
	{
		if (tileNumber + 8 < 56)
			m_vecTile.at(tileNumber + 8)->SetIsSelected();
		if (tileNumber + 16 < 48)
			m_vecTile.at(tileNumber + 16)->SetIsSelected();
	}
	//m_bMove = true;
	m_pSelectedTile = m_vecTile.at(tileNumber);
}

void ChessBoard::SelectOtherTile(int x, int y)
{
	if (IsHavingChessPiece(x, y))
	{
		for (auto iter = m_vecTile.begin(); iter != m_vecTile.end(); iter++)
		{
			if ((*iter)->GetIsSelected())
				(*iter)->SetIsNoSelected();
		}
		ShowMoveableTile(GetSelectedTile(x, y));
	}
}

bool ChessBoard::IsHavingChessPiece(int x, int y)
{
	TILE_STATE selectedTileState = m_vecTile.at(GetSelectedTile(x, y))->GetTileState();
	if (selectedTileState != BLANK)
		return true;
	else
		return false;
}

void ChessBoard::MoveChessPiece(int x, int y)
{
	if (m_vecTile.at(GetSelectedTile(x, y))->GetIsSelected())
	{
		m_vecTile.at(GetSelectedTile(x, y))->SetTileState(m_pSelectedTile->GetTileState());
		m_pSelectedTile->SetTileState(BLANK);

		for (auto iter = m_vecTile.begin(); iter != m_vecTile.end(); iter++)
		{
			if ((*iter)->GetIsSelected())
				(*iter)->SetIsNoSelected();
		}
		m_bTurnEnd = true;
	}
}
