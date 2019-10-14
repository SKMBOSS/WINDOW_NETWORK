#pragma once
#include <Windows.h>
#define MAX_USER 8

class BitMap;
class LobbyRoom
{
private:
	static int	m_sRoomNumber;
	int			m_iThisNumber;
	

	int			m_iUserNum = 8;
	bool		m_bPlaying = false;

	BitMap*		m_pRoomBitMap;
	POINT		m_pos;

	BitMap*		m_pRoomStateBitMap;

public:
	void Init();
	void Update(int x, int y);
	void Render();
	void ProcessPacket(char* szBuf, int len);
public:
	void InitRoom();
	void UpdateRoom(int userNum, bool playing);
	void SelectRoom(int x, int y);
public:
	LobbyRoom();
	~LobbyRoom();
};

