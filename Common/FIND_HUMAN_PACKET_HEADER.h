#pragma once
#include <Windows.h>
#pragma pack(1)

enum PACKET_INDEX
{
	PACKET_INDEX_SEND_USERNAME = 1,
	PACKET_INDEX_SEND_LOGIN_OK,
	PACKET_INDEX_SEND_CHAT,
};

struct PACKET_HEADER
{
	WORD wIndex;
	WORD wLen;
};

struct PACKET_SEND_USERNAME
{
	PACKET_HEADER header;
	int nameLen;
	char name[32];
};

struct PACKET_SEND_LOGIN_OK
{
	PACKET_HEADER header;
	int userIndex;
};

struct PACKET_SEND_CHAT
{
	PACKET_HEADER header;

	int nameLen;
	char name[32];

	int len;
	char szBuf[128];
};
#pragma pack()