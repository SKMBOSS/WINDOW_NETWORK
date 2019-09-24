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
	bool		  m_bTurnEnd;
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
	void SendTurnEnd();

public:
	ChessBoard();
	~ChessBoard();
};

