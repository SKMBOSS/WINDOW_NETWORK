#include "RoomChat.h"
#include "BitMap.h"
#include "ResourceManager.h"
#include "..\..\Common\PACKET_HEADER.h"
#include "UserInfo.h"
#include "Player.h"

RoomChat::RoomChat()
{
}

RoomChat::~RoomChat()
{
}

void RoomChat::Init(HWND hWnd, HINSTANCE hInst)
{
	m_hWnd = hWnd;
	m_hInst = hInst;
	m_msg = "엔터누름";
	m_clearMsg = "";
	m_bKeyFirst = false;

	m_hRoomChatWnd = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		ES_AUTOHSCROLL, 0, 875, 800, 25, hWnd, (HMENU)ID_EDIT, hInst, NULL);
}

void RoomChat::Update()
{
	//키눌림
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (m_bKeyFirst == false)
		{
			char getMsg[128];
			GetDlgItemText(m_hWnd, ID_EDIT, getMsg, 128);
			SendRoomChatMsg(getMsg, lstrlen(getMsg));
			SetDlgItemText(m_hWnd, ID_EDIT, m_clearMsg.c_str());

			m_bKeyFirst = true;
		}
	}
	else
	{
		m_bKeyFirst = false;
	}
}

void RoomChat::Render()
{
	BitMap* window = ResourceManager::GetInstance()->GetBitMap(RES_LOBBY_CHAT);
	BitMap* text = ResourceManager::GetInstance()->GetBitMap(RES_LOBBY_CHAT);
	int msgPosY = 850;

	window->Render(0, 200);
	for (auto iter = m_deqRendrMsg.begin(); iter != m_deqRendrMsg.end(); ++iter)
	{
		text->RenderText(10, msgPosY, (*iter).data());
		msgPosY -= 15;
	}
}

void RoomChat::Relaese()
{}

void RoomChat::SendRoomChatMsg(char* msg, int len)
{
	PACKET_SEND_CHAT packet;
	packet.header.wIndex = PACKET_INDEX_SEND_CHAT;
	packet.header.wLen = sizeof(packet);
	//흠

	for (int i = 0; i < len; i++)
		packet.szBuf[i] = msg[i];

	packet.len = len;
	packet.iIndex = USER_INFO->m_userIndex;
	packet.roomNumber = USER_INFO->m_mapPlayer[USER_INFO->m_userIndex]->roomNumber;

	send(USER_INFO->m_socket, (const char*)&packet, sizeof(packet), 0);
}

void RoomChat::ProcessPacket(char* szBuf, int len)
{
	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_SEND_CHAT:
	{
		PACKET_SEND_CHAT packet;
		memcpy(&packet, szBuf, header.wLen);

		string recvMsg;
		if (packet.iIndex == USER_INFO->m_userIndex)
			recvMsg += (to_string(packet.iIndex) + "번       ME : ");
		else
			recvMsg += (to_string(packet.iIndex) + "번 GUEST : ");

		for (int i = 0; i < packet.len; i++)
			recvMsg += packet.szBuf[i];

		m_deqRendrMsg.push_front(recvMsg);
		if (m_deqRendrMsg.size() == MAX_RENDER_MSG)
		{
			m_deqRendrMsg.pop_back();
		}
	}
	break;
	}
}

