#include "MainGame.h"
#include "Macro.h"
#include "..\..\Common\PACKET_HEADER.h"
#include "UserInfo.h"
#include "Player.h"
#include "BitMap.h"
#include "ResourceManager.h"
#include "ChessBoard.h"

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

void MainGame::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	m_chessBoard = new ChessBoard();
	m_chessBoard->Init();
}

void MainGame::Update(float fElapseTime)
{
	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		POINT pt;
		RECT rc = { 0 , 0 , 800  , 800 };
		GetCursorPos(&pt);
		ScreenToClient(m_hWnd, &pt);

		if (PtInRect(&rc, pt))
		{
			USER_INFO->m_mapPlayer[USER_INFO->m_userIndex]->x = pt.x;
			USER_INFO->m_mapPlayer[USER_INFO->m_userIndex]->y = pt.y;
			SendPos();	
		}
	}
}

void MainGame::SendPos()
{
	PACKET_SEND_POS packet;
	packet.header.wIndex = PACKET_INDEX_SEND_POS;
	packet.header.wLen = sizeof(packet);
	packet.data.iIndex = USER_INFO->m_userIndex;
	packet.data.wX = USER_INFO->m_mapPlayer[USER_INFO->m_userIndex]->x;
	packet.data.wY = USER_INFO->m_mapPlayer[USER_INFO->m_userIndex]->y;
	send(USER_INFO->m_socket, (const char*)&packet, sizeof(packet), 0);
	send(USER_INFO->m_socket, (const char*)&packet, sizeof(packet), 0);
}

void MainGame::Render()
{
	m_chessBoard->Render();
}

void MainGame::Release()
{
	SAFE_RELEASE(m_chessBoard);
	SAFE_DELETE(m_chessBoard);
}

void MainGame::ProcessPacket(char* szBuf, int len)
{
	m_chessBoard->ProcessPacket(szBuf, len);

	PACKET_HEADER header;

	memcpy(&header, szBuf, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_SEND_POS:
	{
		PACKET_SEND_POS packet;
		memcpy(&packet, szBuf, header.wLen);

		m_chessBoard->Update(packet.data.wX, packet.data.wY);
	}
	break;
	}
}