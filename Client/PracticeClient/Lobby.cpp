#include "Lobby.h"
#include "Room.h"
#include "Macro.h"
#include "RoomChat.h"
#include "..\..\Common\PACKET_HEADER.h"
#include "UserInfo.h"
#include "Player.h"
#include "SceneManager.h"

Lobby::Lobby()
{
}


Lobby::~Lobby()
{
}


void Lobby::Init(HWND hWnd, HINSTANCE hInst)
{
	m_hWnd = hWnd;
	m_hInst = hInst;
	m_bKeyFirst = false;

	for (int i = 0; i < 8 * 2; i++)
	{
		Room* room = new Room();
		m_vecRoom.push_back(room);
	}
	for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); iter++)
	{
		(*iter)->Init();
	}

	m_chat = new RoomChat();
	m_chat->Init(hWnd, hInst);

	SendUpdaeRoom();
	Sleep(100);
}
void Lobby::Update(float fElapseTime)
{
	m_chat->Update();

	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		if (m_bKeyFirst == false)
		{
			POINT pt;
			RECT rc = { 0 , 0 , 800  , 800 };
			GetCursorPos(&pt);
			ScreenToClient(m_hWnd, &pt);

			if (PtInRect(&rc, pt))
			{
				for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); iter++)
				{
					(*iter)->Update(pt.x, pt.y);
				}
			}
			m_bKeyFirst = true;
		}
	}
	else
	{
		m_bKeyFirst = false;
	}
}
void Lobby::Render()
{
	for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); ++iter)
		(*iter)->Render();

	m_chat->Render();
}

void Lobby::Release()
{
	for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecRoom.clear();

	SAFE_DELETE(m_chat);
}

void Lobby::SendUpdaeRoom()
{
	PACKET_SEND_UPDATE_ROOM packet;
	packet.header.wIndex = PACKET_INDEX_SEND_UPDATE_ROOM;
	packet.header.wLen = sizeof(packet);
	
	send(USER_INFO->m_socket, (const char*)&packet, sizeof(packet), 0);
}

void Lobby::ProcessPacket(char* szBuf, int len)
{
	m_chat->ProcessPacket(szBuf, len);

	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_SEND_CAHNGE_ROOM:
	{
		SendUpdaeRoom();
		PACKET_SEND_CAHNGE_ROOM packet;
		memcpy(&packet, szBuf, header.wLen);

		USER_INFO->m_mapPlayer[packet.index]->roomNumber = packet.roomNumber;

		if(packet.index == USER_INFO->m_userIndex)
			SceneManager::GetInstance()->ChangeScene(MAINGAME);
	}
	break;
	case PACKET_INDEX_SEND_UPDATE_ROOM:
	{
		PACKET_SEND_UPDATE_ROOM packet;
		memcpy(&packet, szBuf, header.wLen);

		for (int i = 0; i < 16; i++)
			m_vecRoom[i]->SetUserNumCount(packet.userNumCount[i]);
	}
	break;
	}
}