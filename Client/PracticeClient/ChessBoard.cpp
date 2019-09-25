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

	if (USER_INFO->m_userIndex % 2 == 0) //BLACK
	{
		if (selectedTileState == B_PAWN || selectedTileState == B_KNIGHT ||
			selectedTileState == B_BISHOP || selectedTileState == B_ROOK ||
			selectedTileState == B_QUEEN || selectedTileState == B_KING)
			return true;
		else
			return false;
	}
	else //WHITE
	{
		if (selectedTileState == W_PAWN || selectedTileState == W_KNIGHT ||
			selectedTileState == W_BISHOP || selectedTileState == W_ROOK ||
			selectedTileState == W_QUEEN || selectedTileState == W_KING)
			return true;
		else
			return false;
	}
}

void ChessBoard::MoveChessPiece(int x, int y)
{
	if (m_vecTile.at(GetSelectedTile(x, y))->GetIsSelected())
	{
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

//chesePieceMoveable
void ChessBoard::ShowMoveableTile(int tileNumber)
{
	m_iBeforeMovePos = tileNumber;
	TILE_STATE selectedTileState = m_vecTile.at(tileNumber)->GetTileState();

	if (selectedTileState == W_PAWN)
	{
		if (tileNumber + 8 < 64)
			m_vecTile.at(tileNumber + 8)->SetIsSelected();
		if (tileNumber + 16 < 64)
			m_vecTile.at(tileNumber + 16)->SetIsSelected();
	}

	else if (selectedTileState == W_BISHOP)
	{
		ShowMoveableWhiteBishop(tileNumber);
	}

	else if (selectedTileState == W_ROOK)
	{
		ShowMoveableWhiteRook(tileNumber);
	}

	else if (selectedTileState == W_KNIGHT)
	{
		ShowMoveableWhiteKnight(tileNumber);
	}

	else if (selectedTileState == W_QUEEN)
	{
		ShowMoveableWhiteQueen(tileNumber);
	}

	else if (selectedTileState == B_PAWN)
	{
		if (tileNumber - 8 >= 0)
			m_vecTile.at(tileNumber - 8)->SetIsSelected();
		if (tileNumber - 16 >= 0)
			m_vecTile.at(tileNumber - 16)->SetIsSelected();
	}

	else if (selectedTileState == B_BISHOP)
	{
		ShowMoveableBlackBishop(tileNumber);
	}

	else if (selectedTileState == B_ROOK)
	{
		ShowMoveableBlackRook(tileNumber);
	}

	else if (selectedTileState == B_KNIGHT)
	{
		ShowMoveableBlackKnight(tileNumber);
	}

	else if (selectedTileState == B_QUEEN)
	{
		ShowMoveableBlackQueen(tileNumber);
	}
}

void ChessBoard::ShowMoveableWhiteKnight(int tileNumber)
{
	int checkRangeUpDown = tileNumber / 8;
	int checkRangeLeftRight = tileNumber % 8;

	int upLeft = tileNumber - 17;
	int upRight = tileNumber - 15;

	int rightUp = tileNumber - 6;
	int rightDown = tileNumber + 10;

	int downRight = tileNumber + 17;
	int downLeft = tileNumber + 15;

	int leftDown = tileNumber + 6;
	int leftUp = tileNumber - 10;

	if (checkRangeUpDown >= 2)
	{
		if (checkRangeLeftRight >= 1)
		{
			TILE_STATE checkState = m_vecTile.at(upLeft)->GetTileState();
			if (checkState == BLANK || checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
				checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
				m_vecTile.at(upLeft)->SetIsSelected();
		}
		if (checkRangeLeftRight <= 6)
		{
			TILE_STATE checkState = m_vecTile.at(upRight)->GetTileState();
			if (checkState == BLANK || checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
				checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
				m_vecTile.at(upRight)->SetIsSelected();
		}
	}
	if (checkRangeUpDown <= 5)
	{
		if (checkRangeLeftRight >= 1)
		{
			TILE_STATE checkState = m_vecTile.at(downLeft)->GetTileState();
			if (checkState == BLANK || checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
				checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
				m_vecTile.at(downLeft)->SetIsSelected();
		}
		if (checkRangeLeftRight <= 6)
		{
			TILE_STATE checkState = m_vecTile.at(downRight)->GetTileState();
			if (checkState == BLANK || checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
				checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
				m_vecTile.at(downRight)->SetIsSelected();
		}
	}
	if (checkRangeLeftRight <= 5)
	{
		if (checkRangeUpDown >= 1)
		{
			TILE_STATE checkState = m_vecTile.at(rightUp)->GetTileState();
			if (checkState == BLANK || checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
				checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
				m_vecTile.at(rightUp)->SetIsSelected();
		}
		if (checkRangeUpDown <= 6)
		{
			TILE_STATE checkState = m_vecTile.at(rightDown)->GetTileState();
			if (checkState == BLANK || checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
				checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
				m_vecTile.at(rightDown)->SetIsSelected();
		}
	}
	if (checkRangeLeftRight >= 2)
	{
		if (checkRangeUpDown >= 1)
		{
			TILE_STATE checkState = m_vecTile.at(leftUp)->GetTileState();
			if (checkState == BLANK || checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
				checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
				m_vecTile.at(leftUp)->SetIsSelected();
		}
		if (checkRangeUpDown <= 6)
		{
			TILE_STATE checkState = m_vecTile.at(leftDown)->GetTileState();
			if (checkState == BLANK || checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
				checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
				m_vecTile.at(leftDown)->SetIsSelected();
		}
	}
}

void ChessBoard::ShowMoveableBlackKnight(int tileNumber)
{
	int checkRangeUpDown = tileNumber / 8;
	int checkRangeLeftRight = tileNumber % 8;

	int upLeft = tileNumber - 17;
	int upRight = tileNumber - 15;

	int rightUp = tileNumber - 6;
	int rightDown = tileNumber + 10;

	int downRight = tileNumber + 17;
	int downLeft = tileNumber + 15;

	int leftDown = tileNumber + 6;
	int leftUp = tileNumber - 10;

	if (checkRangeUpDown >= 2)
	{
		if (checkRangeLeftRight >= 1)
		{
			TILE_STATE checkState = m_vecTile.at(upLeft)->GetTileState();
			if (checkState == BLANK || checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
				checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
				m_vecTile.at(upLeft)->SetIsSelected();
		}
		if (checkRangeLeftRight <= 6)
		{
			TILE_STATE checkState = m_vecTile.at(upRight)->GetTileState();
			if (checkState == BLANK || checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
				checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
				m_vecTile.at(upRight)->SetIsSelected();
		}
	}
	if (checkRangeUpDown <= 5)
	{
		if (checkRangeLeftRight >= 1)
		{
			TILE_STATE checkState = m_vecTile.at(downLeft)->GetTileState();
			if (checkState == BLANK || checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
				checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
				m_vecTile.at(downLeft)->SetIsSelected();
		}
		if (checkRangeLeftRight <= 6)
		{
			TILE_STATE checkState = m_vecTile.at(downRight)->GetTileState();
			if (checkState == BLANK || checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
				checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
				m_vecTile.at(downRight)->SetIsSelected();
		}
	}
	if (checkRangeLeftRight <= 5)
	{
		if (checkRangeUpDown >= 1)
		{
			TILE_STATE checkState = m_vecTile.at(rightUp)->GetTileState();
			if (checkState == BLANK || checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
				checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
				m_vecTile.at(rightUp)->SetIsSelected();
		}
		if (checkRangeUpDown <= 6)
		{
			TILE_STATE checkState = m_vecTile.at(rightDown)->GetTileState();
			if (checkState == BLANK || checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
				checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
				m_vecTile.at(rightDown)->SetIsSelected();
		}
	}
	if (checkRangeLeftRight >= 2)
	{
		if (checkRangeUpDown >= 1)
		{
			TILE_STATE checkState = m_vecTile.at(leftUp)->GetTileState();
			if (checkState == BLANK || checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
				checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
				m_vecTile.at(leftUp)->SetIsSelected();
		}
		if (checkRangeUpDown <= 6)
		{
			TILE_STATE checkState = m_vecTile.at(leftDown)->GetTileState();
			if (checkState == BLANK || checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
				checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
				m_vecTile.at(leftDown)->SetIsSelected();
		}
	}
}

void ChessBoard::ShowMoveableWhiteRook(int tileNumber)
{
	int up = tileNumber;
	while (true)
	{
		up -= 8;
		if (up < 0)
			break;
		TILE_STATE checkState = m_vecTile.at(up)->GetTileState();
		if (checkState == BLANK)
			m_vecTile.at(up)->SetIsSelected();
		else if (checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
			checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
		{
			m_vecTile.at(up)->SetIsSelected();
			break;
		}
		else
			break;
	}

	int down = tileNumber;
	while (true)
	{
		down += 8;
		if (down > 63)
			break;
		TILE_STATE checkState = m_vecTile.at(down)->GetTileState();
		if (checkState == BLANK)
			m_vecTile.at(down)->SetIsSelected();
		else if (checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
			checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
		{
			m_vecTile.at(down)->SetIsSelected();
			break;
		}
		else
			break;
	}

	int leftRightRange = tileNumber / 8;
	int left = tileNumber;
	while (true)
	{
		left -= 1;
		if (leftRightRange != left / 8 || left < 0)
			break;
		TILE_STATE checkState = m_vecTile.at(left)->GetTileState();
		if (checkState == BLANK)
			m_vecTile.at(left)->SetIsSelected();
		else if (checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
			checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
		{
			m_vecTile.at(left)->SetIsSelected();
			break;
		}
		else
			break;
	}

	int right = tileNumber;
	while (true)
	{
		right += 1;
		if (leftRightRange != right / 8)
			break;
		TILE_STATE checkState = m_vecTile.at(right)->GetTileState();
		if (checkState == BLANK)
			m_vecTile.at(right)->SetIsSelected();
		else if (checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
			checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
		{
			m_vecTile.at(right)->SetIsSelected();
			break;
		}
		else
			break;
	}
}

void ChessBoard::ShowMoveableBlackRook(int tileNumber)
{
	int up = tileNumber;
	while (true)
	{
		up -= 8;
		if (up < 0)
			break;
		TILE_STATE checkState = m_vecTile.at(up)->GetTileState();
		if (checkState == BLANK)
			m_vecTile.at(up)->SetIsSelected();
		else if (checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
			checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
		{
			m_vecTile.at(up)->SetIsSelected();
			break;
		}
		else
			break;
	}

	int down = tileNumber;
	while (true)
	{
		down += 8;
		if (down > 63)
			break;
		TILE_STATE checkState = m_vecTile.at(down)->GetTileState();
		if (checkState == BLANK)
			m_vecTile.at(down)->SetIsSelected();
		else if (checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
			checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
		{
			m_vecTile.at(down)->SetIsSelected();
			break;
		}
		else
			break;
	}

	int leftRightRange = tileNumber / 8;
	int left = tileNumber;
	while (true)
	{
		left -= 1;
		if (leftRightRange != left / 8 || left < 0)
			break;
		TILE_STATE checkState = m_vecTile.at(left)->GetTileState();
		if (checkState == BLANK)
			m_vecTile.at(left)->SetIsSelected();
		else if (checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
			checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
		{
			m_vecTile.at(left)->SetIsSelected();
			break;
		}
		else
			break;
	}

	int right = tileNumber;
	while (true)
	{
		right += 1;
		if (leftRightRange != right / 8)
			break;
		TILE_STATE checkState = m_vecTile.at(right)->GetTileState();
		if (checkState == BLANK)
			m_vecTile.at(right)->SetIsSelected();
		else if (checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
			checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
		{
			m_vecTile.at(right)->SetIsSelected();
			break;
		}
		else
			break;
	}
}

void ChessBoard::ShowMoveableWhiteBishop(int tileNumber)
{
	int crossLeftUp = tileNumber;
	while (true)
	{
		crossLeftUp -= 9;
		if (crossLeftUp < 0)
			break;
		TILE_STATE checkState = m_vecTile.at(crossLeftUp)->GetTileState();
		if (crossLeftUp % 8 == 0 || crossLeftUp / 8 == 0)
		{
			if (checkState == BLANK || checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
				checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
				m_vecTile.at(crossLeftUp)->SetIsSelected();
			break;
		}
		if (checkState == BLANK)
			m_vecTile.at(crossLeftUp)->SetIsSelected();
		else if (checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
			checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
		{
			m_vecTile.at(crossLeftUp)->SetIsSelected();
			break;
		}
		else
			break;
	}

	int crossRightDown = tileNumber;
	while (true)
	{
		crossRightDown += 9;
		if (crossRightDown > 63)
			break;
		TILE_STATE checkState = m_vecTile.at(crossRightDown)->GetTileState();
		if (crossRightDown % 8 == 7 || crossRightDown / 8 == 7)
		{
			if (checkState == BLANK || checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
				checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
				m_vecTile.at(crossRightDown)->SetIsSelected();
			break;
		}
		if (checkState == BLANK)
			m_vecTile.at(crossRightDown)->SetIsSelected();
		else if (checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
			checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
		{
			m_vecTile.at(crossRightDown)->SetIsSelected();
			break;
		}
		else
			break;
	}

	int crossRightUp = tileNumber;
	while (true)
	{
		crossRightUp -= 7;
		if (crossRightUp < 0)
			break;
		TILE_STATE checkState = m_vecTile.at(crossRightUp)->GetTileState();
		if (crossRightUp % 8 == 7 || crossRightUp / 8 == 0)
		{
			if (checkState == BLANK || checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
				checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
				m_vecTile.at(crossRightUp)->SetIsSelected();
			break;
		}
		if (checkState == BLANK)
			m_vecTile.at(crossRightUp)->SetIsSelected();
		else if (checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
			checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
		{
			m_vecTile.at(crossRightUp)->SetIsSelected();
			break;
		}
		else
			break;
	}

	int crossLeftDown = tileNumber;
	while (true)
	{
		crossLeftDown += 7;
		if (crossLeftDown > 63)
			break;
		TILE_STATE checkState = m_vecTile.at(crossLeftDown)->GetTileState();
		if (crossLeftDown % 8 == 0 || crossLeftDown / 8 == 7)
		{
			if (checkState == BLANK || checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
				checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
				m_vecTile.at(crossLeftDown)->SetIsSelected();
			break;
		}
		if (checkState == BLANK)
			m_vecTile.at(crossLeftDown)->SetIsSelected();
		else if (checkState == B_PAWN || checkState == B_KNIGHT || checkState == B_BISHOP ||
			checkState == B_ROOK || checkState == B_QUEEN || checkState == B_KING)
		{
			m_vecTile.at(crossLeftDown)->SetIsSelected();
			break;
		}
		else
			break;
	}
}

void ChessBoard::ShowMoveableBlackBishop(int tileNumber)
{
	int crossLeftUp = tileNumber;
	while (true)
	{
		crossLeftUp -= 9;
		if (crossLeftUp < 0)
			break;
		TILE_STATE checkState = m_vecTile.at(crossLeftUp)->GetTileState();
		if (crossLeftUp % 8 == 0 || crossLeftUp / 8 == 0)
		{
			if (checkState == BLANK || checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
				checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
				m_vecTile.at(crossLeftUp)->SetIsSelected();
			break;
		}
		if (checkState == BLANK)
			m_vecTile.at(crossLeftUp)->SetIsSelected();
		else if (checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
			checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
		{
			m_vecTile.at(crossLeftUp)->SetIsSelected();
			break;
		}
		else
			break;
	}

	int crossRightDown = tileNumber;
	while (true)
	{
		crossRightDown += 9;
		if (crossRightDown > 63)
			break;
		TILE_STATE checkState = m_vecTile.at(crossRightDown)->GetTileState();
		if (crossRightDown % 8 == 7 || crossRightDown / 8 == 7)
		{
			if (checkState == BLANK || checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
				checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
				m_vecTile.at(crossRightDown)->SetIsSelected();
			break;
		}
		if (checkState == BLANK)
			m_vecTile.at(crossRightDown)->SetIsSelected();
		else if (checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
			checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
		{
			m_vecTile.at(crossRightDown)->SetIsSelected();
			break;
		}
		else
			break;
	}

	int crossRightUp = tileNumber;
	while (true)
	{
		crossRightUp -= 7;
		if (crossRightUp < 0)
			break;
		TILE_STATE checkState = m_vecTile.at(crossRightUp)->GetTileState();
		if (crossRightUp % 8 == 7 || crossRightUp / 8 == 0)
		{
			if (checkState == BLANK || checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
				checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
				m_vecTile.at(crossRightUp)->SetIsSelected();
			break;
		}
		if (checkState == BLANK)
			m_vecTile.at(crossRightUp)->SetIsSelected();
		else if (checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
			checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
		{
			m_vecTile.at(crossRightUp)->SetIsSelected();
			break;
		}
		else
			break;
	}

	int crossLeftDown = tileNumber;
	while (true)
	{
		crossLeftDown += 7;
		if (crossLeftDown > 63)
			break;
		TILE_STATE checkState = m_vecTile.at(crossLeftDown)->GetTileState();
		if (crossLeftDown % 8 == 0 || crossLeftDown / 8 == 7)
		{
			if (checkState == BLANK || checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
				checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
				m_vecTile.at(crossLeftDown)->SetIsSelected();
			break;
		}
		if (checkState == BLANK)
			m_vecTile.at(crossLeftDown)->SetIsSelected();
		else if (checkState == W_PAWN || checkState == W_KNIGHT || checkState == W_BISHOP ||
			checkState == W_ROOK || checkState == W_QUEEN || checkState == W_KING)
		{
			m_vecTile.at(crossLeftDown)->SetIsSelected();
			break;
		}
		else
			break;
	}
}

void ChessBoard::ShowMoveableWhiteQueen(int tileNumber)
{
	ShowMoveableWhiteBishop(tileNumber);
	ShowMoveableWhiteRook(tileNumber);
}
void ChessBoard::ShowMoveableBlackQueen(int tileNumber)
{
	ShowMoveableBlackBishop(tileNumber);
	ShowMoveableBlackRook(tileNumber);
}

