#pragma once
#include <Windows.h>
#pragma pack(1)

enum PACKET_INDEX
{
	PACKET_INDEX_LOGIN_RET = 1,
	PACKET_INDEX_USER_DATA,
	PACKET_INDEX_SEND_TURN_END,
	PACKET_INDEX_SEND_CHAT
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
};

struct PACKET_SEND_CHAT
{
	PACKET_HEADER header;
	char szBuf[128];
	int len;
	int iIndex;
	int roomNumber;
};


#pragma pack()