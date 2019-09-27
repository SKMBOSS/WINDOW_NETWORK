#pragma once
#include <Windows.h>

enum ROOM_STATE
{
	WAIT,
	PLAYING,
};


class BitMap;
class Room
{
private:
	static int	m_sRoomNumber;
	int			m_iThisNumber;
	POINT		m_pos;
	BitMap*		m_pRoomBitMap;

	ROOM_STATE	m_eRoomState;
	int			m_userNumCount;

public:
	void Init();
	void Render();
	void Update(int x, int y);
public:
	void InitRoom();
	void SelectRoom(int x, int y);
	void SetUserNumCount(int count);
public:
	Room();
	~Room();
};

