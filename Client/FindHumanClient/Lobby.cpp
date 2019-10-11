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

	SendUserViewUpdate();
}

void Lobby::Update(float fElapseTime)
{
	m_pChat->Update();

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
}

void Lobby::SendUserViewUpdate()
{
	PACKET_SEND_USER_VIEW packet;
	packet.header.wIndex = PACKET_INDEX_SEND_USER_VIEW;
	packet.header.wLen = sizeof(PACKET_HEADER);
	
	send(USER_INFO->m_socket, (const char*)&packet, packet.header.wLen, 0);
}