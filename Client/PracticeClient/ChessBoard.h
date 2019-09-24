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
	void ShowMoveableTile(int tileNumber);
	void SelectOtherTile(int x, int y);
	bool IsHavingChessPiece(int x, int y);
	void MoveChessPiece(int x, int y);
public:
	void ProcessPacket(char* szBuf, int len);
	void SendTurnEnd(int before, int after);
	void UpdateBoard(int before, int after);

public:
	ChessBoard();
	~ChessBoard();
};

