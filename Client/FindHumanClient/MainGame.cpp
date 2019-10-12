#include "MainGame.h"
#include "Macro.h"
#include "..\..\Common\FIND_HUMAN_PACKET_HEADER.h"
#include "SceneManager.h"
#include "BackGround.h"
#include "Chat.h"
#include "UserView.h"
#include "LobbyRoom.h"
#include "UserInfo.h"
#include "PaintBoard.h"

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

	m_pPaintBoard = new PaintBoard;
	m_pPaintBoard->Init(GetDC(hWnd));

	SendUserViewUpdate();
	m_pPaintBoard->Render();
}

void MainGame::Update(float fElapseTime)
{
	m_pChat->Update();

	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		POINT pt;
		RECT rc = { 20 , 60 , 760  , 500 };
		GetCursorPos(&pt);
		ScreenToClient(m_hWnd, &pt);

		if (PtInRect(&rc, pt))
		{
			SendPixelPos(pt.x, pt.y);
		}
	}
}

void MainGame::Render()
{
	m_pBackGround->Render();
	m_pUserView->Render();
	m_pChat->Render();
	m_pPaintBoard->Render();
}

void MainGame::Release()
{
	SAFE_RELEASE(m_pPaintBoard);
	SAFE_DELETE(m_pPaintBoard);
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

	switch (header.wIndex)
	{
	case PACKET_INDEX_SEND_PIXEL_POS:
	{
		PACKET_SEND_PIXEL_POS packet;
		memcpy(&packet, szBuf, header.wLen);

		m_pPaintBoard->Update(packet.pos.x, packet.pos.y);
	}
	break;
	}
}

void MainGame::SendUserViewUpdate()
{
	PACKET_SEND_USER_VIEW packet;
	packet.header.wIndex = PACKET_INDEX_SEND_USER_VIEW;
	packet.header.wLen = sizeof(PACKET_HEADER);

	send(USER_INFO->m_socket, (const char*)&packet, packet.header.wLen, 0);
}

void MainGame::SendPixelPos(int x, int y)
{
	PACKET_SEND_PIXEL_POS packet;
	packet.header.wIndex = PACKET_INDEX_SEND_PIXEL_POS;
	packet.header.wLen = sizeof(packet);

	packet.pos.x = x;
	packet.pos.y = y;

	send(USER_INFO->m_socket, (const char*)&packet, packet.header.wLen, 0);
}