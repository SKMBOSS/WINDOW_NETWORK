#pragma once
#include <Windows.h>
#include <vector>
using namespace std;

class Tile;
class ChessBoard
{
private:
	vector<Tile*> m_vecTile;
public:
	void Init();
	void Render();
	void Update(int x, int y);
	void Release();
public:
	ChessBoard();
	~ChessBoard();
};

