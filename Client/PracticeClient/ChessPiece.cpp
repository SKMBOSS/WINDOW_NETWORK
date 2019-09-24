#include "ChessPiece.h"
#include "ChessBoard.h"
#include "Tile.h"

ChessBoard* ChessPiece::m_chessBoard = nullptr;

ChessPiece::ChessPiece()
{
}

ChessPiece::~ChessPiece()
{
}

void ChessPiece::SetPos(Tile* tile)
{
	m_pos = tile->GetPos();
}
