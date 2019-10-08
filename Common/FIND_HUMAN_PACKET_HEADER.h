#pragma once
#include <Windows.h>
#pragma pack(1)

enum PACKET_INDEX
{
	PACKET_INDEX_LOGIN = 1,
	PACKET_INDEX_USER_DATA,
	PACKET_INDEX_SEND_DELETE_USER,
	PACKET_INDEX_SEND_UPDATE_USER_VIEW,
	PACKET_INDEX_SEND_USERNAME,
	PACKET_INDEX_SEND_LOGIN_OK
};

struct PACKET_HEADER
{
	WORD wIndex;
	WORD wLen;
};

struct USER_DATA
{
	int iIndex;
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
	USER_DATA data[40];
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

struct PACKET_SEND_USERNAME
{
	PACKET_HEADER header;
	int userIndex;
	int roomNumber;
	int nameLen;
	char name[32];
};

struct PACKET_SEND_LOGIN_OK
{
	PACKET_HEADER header;
	int userIndex;
};
#pragma pack()