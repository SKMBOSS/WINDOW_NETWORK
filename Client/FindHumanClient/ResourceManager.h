#pragma once
#include <Windows.h>
#include <vector>
#include <string>
using namespace std;

enum RES_TYPE
{
	//BackGround
	RES_BACKGROUND,
	
	//Chat
	RES_CHAT_BAR,

	//UserView
	RES_USER_VIEW,

	//LobbyRoom
	RES_LOBBY_ROOM,
	RES_WORD_WAITING,
	RES_WORD_FULL,
	RES_WORD_PLAYING,

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