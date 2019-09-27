#pragma once
#include <Windows.h>
#include <vector>
#include <string>
using namespace std;

enum RES_TYPE
{
	//board
	RES_BOARD_WHITE,
	RES_BOARD_BLACK,
	RES_BOARD_SELECTED,
	//white
	RES_WHITE_PAWN,
	RES_WHITE_KNIGHT,
	RES_WHITE_BISHOP,
	RES_WHITE_ROOK,
	RES_WHITE_QUEEN,
	RES_WHITE_KING,
	//black
	RES_BLACK_PAWN,
	RES_BLACK_KNIGHT,
	RES_BLACK_BISHOP,
	RES_BLACK_ROOK,
	RES_BLACK_QUEEN,
	RES_BLACK_KING,
	//end
	RES_NULL,
	RES_LOBBY_CHAT,
	RES_TYPE_END
};

class BitMap;
class ResourceManager
{
private:
	static ResourceManager* m_sThis;
	BitMap* m_pBitMap[RES_TYPE_END];
	vector<string> resPath;
public:
	static ResourceManager* GetInstance();
	BitMap* GetBitMap(int index);
	SIZE GetBitMapSize(int index);
private:
	void InitResPath();
	void InitBitMap(HDC hdc);
public:
	void Init(HDC hdc);
	void Release();
private:
	ResourceManager();
public:
	~ResourceManager();
};

