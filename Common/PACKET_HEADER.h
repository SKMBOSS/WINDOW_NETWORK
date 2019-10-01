#pragma once
#include <Windows.h>
#pragma pack(1)

enum PACKET_INDEX
{
	PACKET_INDEX_LOGIN_RET = 1,
	PACKET_INDEX_USER_DATA,
	PACKET_INDEX_SEND_TURN_END,
	PACKET_INDEX_SEND_CHAT,
	PACKET_INDEX_SEND_CAHNGE_ROOM,
	PACKET_INDEX_SEND_UPDATE_ROOM,
	PACKET_INDEX_SEND_TURN_INIT,
	PACKET_INDEX_SEND_READY,
	PACKET_INDEX_SEND_GAME_END,
	PACKET_INDEX_SEND_GAME_ROOM_EXIT,
	PACKET_INDEX_SEND_DELETE_USER,
	PACKET_INDEX_SEND_UPDATE_USER_VIEW
};

struct PACKET_HEADER
{
	WORD wIndex;
	WORD wLen;
};

struct USER_DATA
{
	int iIndex;
	int roomNumber;
	bool turn;
};

struct PACKET_LOGIN_RET
{
	PACKET_HEADER header;
	int iIndex;
};

struct PACKET_USER_DATA
{
	PACKET_HEADER header;
	WORD wCount;
	USER_DATA data[20];
};

struct PACKET_SEND_TURNEND
{
	PACKET_HEADER header;
	USER_DATA data;
	int beforePos;
	int afterPos;
	int deadChessPiece;
};

struct PACKET_SEND_CHAT
{
	PACKET_HEADER header;
	
	int iIndex;
	int roomNumber;
	int color;
	int len;
	char szBuf[128];
};

struct PACKET_SEND_CAHNGE_ROOM
{
	PACKET_HEADER header;
	int index;
	int roomNumber;
};

struct PACKET_SEND_UPDATE_ROOM
{
	PACKET_HEADER header;
	int userNumCount[16];
};

struct PACKET_SEND_TURN_INIT
{
	PACKET_HEADER header;
	USER_DATA data;
	int color;
};

struct PACKET_SEND_READY
{
	PACKET_HEADER header;
	int roomNumber;
	bool ready;
};

struct PACKET_SEND_GAME_END
{
	PACKET_HEADER header;
};

struct PACKET_SEND_GAME_ROOM_EXIT
{
	PACKET_HEADER header;
};

struct PACKET_SEND_DELETE_USER
{
	PACKET_HEADER header;
	int index;
};

struct PACKET_SEND_UPDATE_USER_VIEW
{
	PACKET_HEADER header;
	int index;
	int roomNumber;
};


#pragma pack()