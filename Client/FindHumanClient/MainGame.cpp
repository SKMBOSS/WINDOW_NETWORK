#include "MainGame.h"
#include "Macro.h"
#include "..\..\Common\FIND_HUMAN_PACKET_HEADER.h"
#include "SceneManager.h"
#include "BackGround.h"
#include "Chat.h"
#include "UserView.h"
#include "LobbyRoom.h"
#include "UserInfo.h"

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

void MainGame::Init(HWND hWnd, HINSTANCE hInst)
{
	m_hWnd = hWnd;
	m_hInst = hInst;

	m_pBackGround = new BackGround;
	m_pBackGround->Init();

	m_pUserView = new UserView;
	m_pUserView->Init();

	m_pChat = new Chat;
	m_pChat->Init(hWnd, hInst, 16);

	SendUserViewUpdate();
}

void MainGame::Update(float fElapseTime)
{
	m_pChat->Update();
}

void MainGame::Render()
{
	m_pBackGround->Render();
	m_pUserView->Render();
	m_pChat->Render();
}

void MainGame::Release()
{
	SAFE_DELETE(m_pChat);
	SAFE_DELETE(m_pUserView);
	SAFE_DELETE(m_pBackGround);
}

void MainGame::ProcessPacket(char* szBuf, int len)
{
	m_pUserView->ProcessPacket(szBuf, len);
	m_pChat->ProcessPacket(szBuf, len);

	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));
}

void MainGame::SendUserViewUpdate()
{
	PACKET_SEND_USER_VIEW packet;
	packet.header.wIndex = PACKET_INDEX_SEND_USER_VIEW;
	packet.header.wLen = sizeof(PACKET_HEADER);

	send(USER_INFO->m_socket, (const char*)&packet, packet.header.wLen, 0);
}