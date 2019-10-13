#include "MainGame.h"
#include "Macro.h"
#include "..\..\Common\CATCH_MIND_PACKET_HEADER.h"
#include "SceneManager.h"
#include "BackGround.h"
#include "Chat.h"
#include "UserView.h"
#include "LobbyRoom.h"
#include "SocketManager.h"
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
	m_pPaintBoard->Init();

	SendUserViewUpdate();
}

void MainGame::Update(float fElapseTime)
{
	m_pChat->Update();
	m_pPaintBoard->Update(m_hWnd);
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
	m_pPaintBoard->ProcessPacket(szBuf, len);

	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	/*switch (header.wIndex)
	{
	case PACKET_INDEX_SEND_PIXEL_POS:
	{
		PACKET_SEND_PIXEL_POS packet;
		memcpy(&packet, szBuf, header.wLen);

		m_pPaintBoard->Update(packet.pos.x, packet.pos.y);
	}
	break;
	}*/
}

void MainGame::SendUserViewUpdate()
{
	PACKET_SEND_USER_VIEW packet;
	packet.header.wIndex = PACKET_INDEX_SEND_USER_VIEW;
	packet.header.wLen = sizeof(PACKET_HEADER);

	send(SOCKET_INFO->m_socket, (const char*)&packet, packet.header.wLen, 0);
}