#include "BlackPawn.h"
#include "ResourceManager.h"
#include "BitMap.h"
#include "Tile.h"
#include "ChessBoard.h"



BlackPawn::BlackPawn()
{
}

BlackPawn::~BlackPawn()
{
}

void BlackPawn::init(Tile* tile)
{
	ChessPiece::m_pChessPieceBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BLACK_PAWN);
	ChessPiece::m_currentTile = tile;
	ChessPiece::SetPos(tile);
}

void BlackPawn::Update()
{
	//
}

void BlackPawn::Render()
{
	m_pChessPieceBitMap->Render(ChessPiece::m_pos.x, ChessPiece::m_pos.y);
}

void BlackPawn::Move()
{

	int iCurTileNum = ChessPiece::m_currentTile->GetTileNumber();

	if (iCurTileNum >= 16)
		ChessPiece::m_currentTile = ChessPiece::m_chessBoard->GetTile(iCurTileNum - 8);
}


