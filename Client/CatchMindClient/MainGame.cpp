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
#include "ReadyButton.h"
#include "StartButton.h"
#include "UserReadyBox.h"

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

	m_pReadyButton = new ReadyButton;
	m_pReadyButton->Init(hWnd);

	m_pStartButton = new StartButton;
	m_pStartButton->Init(hWnd);

	m_pUserView = new UserView;
	m_pUserView->Init();

	m_pUserReadyBox = new UserReadyBox;
	m_pUserReadyBox->Init();

	m_pChat = new Chat;
	m_pChat->Init(hWnd, hInst, 16);

	m_pPaintBoard = new PaintBoard;
	m_pPaintBoard->Init();

	m_bReadyButtonActive = true;
	m_bStartButtonActive = false;

	SendUserViewUpdate();
}

void MainGame::Update(float fElapseTime)
{
	if (m_bReadyButtonActive)
		m_pReadyButton->Update();

	if (m_bStartButtonActive)
		m_pStartButton->Update();

	m_pChat->Update();
	m_pPaintBoard->Update(m_hWnd);
}

void MainGame::Render()
{
	m_pBackGround->Render();

	if (m_bReadyButtonActive)
		m_pReadyButton->Render();

	if (m_bStartButtonActive)
		m_pStartButton->Render();

	m_pUserView->Render();

	if (m_bReadyButtonActive)
		m_pUserReadyBox->Render();

	m_pChat->Render();
	m_pPaintBoard->Render();
}

void MainGame::Release()
{
	SAFE_RELEASE(m_pPaintBoard);
	SAFE_DELETE(m_pPaintBoard);
	SAFE_DELETE(m_pChat);
	SAFE_DELETE(m_pUserView);
	SAFE_DELETE(m_pStartButton);
	SAFE_DELETE(m_pReadyButton);
	SAFE_DELETE(m_pBackGround);
}

void MainGame::ProcessPacket(char* szBuf, int len)
{
	m_pUserView->ProcessPacket(szBuf, len);
	m_pUserReadyBox->ProcessPacket(szBuf, len);
	m_pChat->ProcessPacket(szBuf, len);
	m_pPaintBoard->ProcessPacket(szBuf, len);

	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_SEND_READY_BUTTON_CLICK:
	{
		PACKET_SEND_READY_BUTTON_CLICK packet;
		memcpy(&packet, szBuf, header.wLen);

		SendUserReadyBoxUpdate();
	}
	break;
	case PACKET_INDEX_SEND_START_BUTTON_CLICK:
	{
		PACKET_SEND_START_BUTTON_CLICK packet;
		memcpy(&packet, szBuf, header.wLen);

		m_bReadyButtonActive = false;
		m_bStartButtonActive = false;

		m_pPaintBoard->SendAllDelete();
	}
	break;

	case PACKET_INDEX_SEND_GAME_READY_TRUE:
	{
		PACKET_SEND_GAME_READY_TRUE packet;
		memcpy(&packet, szBuf, header.wLen);

		m_bStartButtonActive = true;
	}
	break;
	case PACKET_INDEX_SEND_GAME_READY_FALSE:
	{
		PACKET_SEND_GAME_READY_FALSE packet;
		memcpy(&packet, szBuf, header.wLen);

		m_bStartButtonActive = false;
	}
	break;
	}
}

void MainGame::SendUserViewUpdate()
{
	PACKET_SEND_USER_VIEW packet;
	packet.header.wIndex = PACKET_INDEX_SEND_USER_VIEW;
	packet.header.wLen = sizeof(PACKET_HEADER);

	send(SOCKET_INFO->m_socket, (const char*)&packet, packet.header.wLen, 0);
}

void MainGame::SendUserReadyBoxUpdate()
{
	PACKET_SEND_USER_READY_BOX packet;
	packet.header.wIndex = PACKET_INDEX_SEND_USER_READY_BOX;
	packet.header.wLen = sizeof(PACKET_HEADER);

	send(SOCKET_INFO->m_socket, (const char*)&packet, packet.header.wLen, 0);
}