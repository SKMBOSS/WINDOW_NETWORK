#pragma once
#include <Windows.h>
#include <vector>
using namespace std;

class Tile;
class ChessBoard
{
private:
	vector<Tile*> m_vecTile;
	Tile*		  m_pSelectedTile;
	int			  m_iBeforeMovePos;
	int			  m_iAfterMovePos;
public:
	void Init();
	void Render();
	void Update(int x, int y);
	void Release();
public:
	int GetSelectedTile(int x, int y);
	void SelectOtherTile(int x, int y);
	bool IsHavingChessPiece(int x, int y);
	void MoveChessPiece(int x, int y);
public:
	void ShowMoveableTile(int tileNumber);

	void ShowMoveableWhitePawn(int tileNumber);
	void ShowMoveableBlackPawn(int tileNumber);

	void ShowMoveableWhiteKnight(int tileNumber);
	void ShowMoveableBlackKnight(int tileNumber);

	void ShowMoveableWhiteBishop(int tileNumber);
	void ShowMoveableBlackBishop(int tileNumber);

	void ShowMoveableWhiteRook(int tileNumber);
	void ShowMoveableBlackRook(int tileNumber);

	void ShowMoveableWhiteQueen(int tileNumber);
	void ShowMoveableBlackQueen(int tileNumber);

	void ShowMoveableWhiteKing(int tileNumber);
	void ShowMoveableBlackKing(int tileNumber);

public:
	void ProcessPacket(char* szBuf, int len);
	void SendTurnEnd(int before, int after);
	void UpdateBoard(int before, int after);

public:
	ChessBoard();
	~ChessBoard();
};

