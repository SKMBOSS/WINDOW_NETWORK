#pragma once
#include <Windows.h>
#define MAX_USER 8

enum ROOM_STATE
{
	ROOM_WAITING,
	ROOM_FULL,
	ROOM_PLAYING,
};

class BitMap;
class LobbyRoom
{
private:
	static int	m_sRoomNumber;
	int			m_iThisNumber;
	POINT		m_pos;

	int			m_iUserNum =8;

	BitMap*		m_pRoomBitMap;
	BitMap*		m_pRoomStateBitMap;

	ROOM_STATE	m_eRoomState;

public:
	void Init();
	void Update(int x, int y);
	void Render();
	void ProcessPacket(char* szBuf, int len);
public:
	void InitRoom();
	void UpdateRoom(int userNum);
	void SelectRoom(int x, int y);
public:
	LobbyRoom();
	~LobbyRoom();
};

