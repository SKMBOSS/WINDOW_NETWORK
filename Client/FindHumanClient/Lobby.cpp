#include "Lobby.h"
#include "Macro.h"
#include "..\..\Common\FIND_HUMAN_PACKET_HEADER.h"
#include "SceneManager.h"
#include "BackGround.h"
#include "Chat.h"
#include "UserView.h"
#include "LobbyRoom.h"
#include "UserInfo.h"

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

	m_pBackGround = new BackGround;
	m_pBackGround->Init();

	m_pUserView = new UserView;
	m_pUserView->Init();

	m_pChat = new Chat;
	m_pChat->Init(hWnd, hInst, 16);

	for (int i = 0; i < 12; i++)
	{
		LobbyRoom* room = new LobbyRoom();
		m_vecRoom.push_back(room);
	}
	for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); iter++)
	{
		(*iter)->Init();
	}
		

	SendLobbyRoomUpdate();
	SendUserViewUpdate();
}

void Lobby::Update(float fElapseTime)
{
	m_pChat->Update();

	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		if (m_bKeyFirst == false)
		{
			POINT pt;
			RECT rc = { 0 , 0 , 900  , 900 };
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
	m_pBackGround->Render();
	m_pUserView->Render();
	m_pChat->Render();
	for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); ++iter)
		(*iter)->Render();
}

void Lobby::Release()
{
	for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
	SAFE_DELETE(m_pChat);
	SAFE_DELETE(m_pUserView);
	SAFE_DELETE(m_pBackGround);
}

void Lobby::ProcessPacket(char* szBuf, int len)
{
	m_pUserView->ProcessPacket(szBuf, len);
	m_pChat->ProcessPacket(szBuf, len);
	
	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_SEND_UPDATE_LOBBY_ROOM:
	{
		PACKET_SEND_UPDATE_LOBBY_ROOM packet;
		memcpy(&packet, szBuf, header.wLen);

		int i = 0;
		for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); ++iter)
		{
			(*iter)->UpdateRoom(packet.arrUserNum[i]) ;
			i++;
		}
	}
	break;
	case PACKET_INDEX_SEND_CAHNGE_ROOM:
	{
		PACKET_SEND_CAHNGE_ROOM packet;
		memcpy(&packet, szBuf, header.wLen);

		Sleep(500);
		SceneManager::GetInstance()->ChangeScene(MAINGAME);
	}
	}
}

void Lobby::SendUserViewUpdate()
{
	PACKET_SEND_USER_VIEW packet;
	packet.header.wIndex = PACKET_INDEX_SEND_USER_VIEW;
	packet.header.wLen = sizeof(PACKET_HEADER);
	
	send(USER_INFO->m_socket, (const char*)&packet, packet.header.wLen, 0);
}

void Lobby::SendLobbyRoomUpdate()
{
	PACKET_SEND_UPDATE_LOBBY_ROOM packet;
	packet.header.wIndex = PACKET_INDEX_SEND_UPDATE_LOBBY_ROOM;
	packet.header.wLen = sizeof(PACKET_HEADER);

	send(USER_INFO->m_socket, (const char*)&packet, packet.header.wLen, 0);
}