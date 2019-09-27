#include "MainGame.h"
#include "Macro.h"
#include "..\..\Common\PACKET_HEADER.h"
#include "UserInfo.h"
#include "Player.h"
#include "BitMap.h"
#include "ResourceManager.h"
#include "ChessBoard.h"
#include "Chat.h"

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
	m_chessBoard = new ChessBoard();
	m_chat = new Chat();

	m_chessBoard->Init();
	m_chat->Init(hWnd, hInst);
}

void MainGame::Update(float fElapseTime)
{
	m_chat->Update();

	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		POINT pt;
		RECT rc = { 0 , 0 , 800  , 800 };
		GetCursorPos(&pt);
		ScreenToClient(m_hWnd, &pt);

		if (PtInRect(&rc, pt))
		{
			if (USER_INFO->m_mapPlayer[USER_INFO->m_userIndex]->turn)
				m_chessBoard->Update(pt.x, pt.y);
		}
	}
}

void MainGame::Render()
{
	m_chessBoard->Render();
	m_chat->Render();
}

void MainGame::Release()
{
	SAFE_RELEASE(m_chessBoard);
	SAFE_DELETE(m_chessBoard);

	SAFE_DELETE(m_chat);
}

void MainGame::ProcessPacket(char* szBuf, int len)
{
	m_chessBoard->ProcessPacket(szBuf, len);
	m_chat->ProcessPacket(szBuf, len);

	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_SEND_TURN_END:
	{
		PACKET_SEND_TURNEND packet;
		memcpy(&packet, szBuf, header.wLen);

		USER_INFO->m_mapPlayer[USER_INFO->m_userIndex]->turn = packet.data.turn;
		m_chessBoard->UpdateBoard(packet.beforePos, packet.afterPos);
	}
	break;
	}
}