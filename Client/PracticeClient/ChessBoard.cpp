#include "ChessBoard.h"
#include "Macro.h"
#include "Tile.h"
#include "UserInfo.h"
#include "Player.h"
#include "..\..\Common\PACKET_HEADER.h"

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
	if (USER_INFO->m_mapPlayer[USER_INFO->m_userIndex]->turn)
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
	//m_pSelectedTile = m_vecTile.at(tileNumber);
	m_iBeforeMovePos = tileNumber;
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
		//m_vecTile.at(GetSelectedTile(x, y))->SetTileState(m_pSelectedTile->GetTileState());
		//m_pSelectedTile->SetTileState(BLANK);
		m_iAfterMovePos = GetSelectedTile(x, y);
		for (auto iter = m_vecTile.begin(); iter != m_vecTile.end(); iter++)
		{
			if ((*iter)->GetIsSelected())
				(*iter)->SetIsNoSelected();
		}
		SendTurnEnd(m_iBeforeMovePos, m_iAfterMovePos);
	}
}

void ChessBoard::SendTurnEnd(int before, int after)
{
	PACKET_SEND_TURNEND packet;
	packet.header.wIndex = PACKET_INDEX_SEND_TURN_END;
	packet.header.wLen = sizeof(packet);
	//Èì
	packet.data.iIndex = -1;
	packet.data.turn = false;
	packet.beforePos = before;
	packet.afterPos = after;
	
	send(USER_INFO->m_socket, (const char*)&packet, sizeof(packet), 0);
}

void ChessBoard::UpdateBoard(int before, int after)
{
	m_vecTile.at(after)->SetTileState(m_vecTile.at(before)->GetTileState());
	m_vecTile.at(before)->SetTileState(BLANK);
}

void ChessBoard::ProcessPacket(char* szBuf, int len)
{
	//
}
