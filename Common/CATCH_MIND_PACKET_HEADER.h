#pragma once
#include <Windows.h>
#pragma pack(1)

enum PACKET_INDEX
{
	PACKET_INDEX_SEND_USERNAME = 1,
	PACKET_INDEX_SEND_LOGIN_OK,
	PACKET_INDEX_SEND_CHAT,
	PACKET_INDEX_SEND_USER_VIEW,
	PACKET_INDEX_SEND_UPDATE_LOBBY_ROOM,
	PACKET_INDEX_SEND_CAHNGE_ROOM,
	PACKET_INDEX_SEND_PIXEL_POS,
	PACKET_INDEX_SEND_ALL_DELETE,
	PACKET_INDEX_SEND_READY_BUTTON_CLICK,
	PACKET_INDEX_SEND_GAME_READY_TRUE,
	PACKET_INDEX_SEND_GAME_READY_FALSE
};

struct PACKET_HEADER
{
	WORD wIndex;
	WORD wLen;
};

struct PACKET_SEND_USERNAME
{
	PACKET_HEADER	header;
	int				nameLen;
	char			name[32];
};

struct PACKET_SEND_LOGIN_OK
{
	PACKET_HEADER	header;
	int				userIndex;
};

struct PACKET_SEND_CHAT
{
	PACKET_HEADER	header;
	int				nameLen;
	char			name[32];
	int				len;
	char			szBuf[128];
};

struct PACKET_SEND_USER_VIEW
{
	PACKET_HEADER	header;
	int				userNum;
	int				len;
	char			 szBuf[512];
};

struct PACKET_SEND_UPDATE_LOBBY_ROOM
{
	PACKET_HEADER	header;
	int				arrUserNum[12];
};

struct PACKET_SEND_CAHNGE_ROOM
{
	PACKET_HEADER	header;
	int				roomNumber;
};

struct PACKET_SEND_PIXEL_POS
{
	PACKET_HEADER	header;
	POINT			newPos;
	POINT			oldPos;
	int				pixelWidth;
	COLORREF		color;
};

struct PACKET_SEND_ALL_DELETE
{
	PACKET_HEADER	header;
};

struct PACKET_SEND_READY_BUTTON_CLICK
{
	PACKET_HEADER	header;
};

struct PACKET_SEND_GAME_READY_TRUE
{
	PACKET_HEADER	header;
};

struct PACKET_SEND_GAME_READY_FALSE
{
	PACKET_HEADER	header;
};


#pragma pack()