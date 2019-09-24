#pragma once
#include <Windows.h>

class BitMap;
class Tile;
class ChessBoard;
class ChessPiece
{
public:
	static ChessBoard*  m_chessBoard;
protected:
	BitMap*				m_pChessPieceBitMap;
	POINT				m_pos;
	Tile*				m_currentTile;
public:
	virtual void init(Tile* tile) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Move() = 0;
public:
	//virtual void Release() = 0;
public:
	void SetPos(Tile* tile);

	ChessPiece();
	~ChessPiece();
};

