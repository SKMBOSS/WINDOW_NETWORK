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

	int			m_iUserNum = 0;

	BitMap*		m_pRoomBitMap;
	BitMap*		m_pRoomStateBitMap;

	ROOM_STATE	m_eRoomState;

public:
	void Init();
	void Render();
	void ProcessPacket(char* szBuf, int len);
public:
	void InitRoom();
public:
	LobbyRoom();
	~LobbyRoom();
};

