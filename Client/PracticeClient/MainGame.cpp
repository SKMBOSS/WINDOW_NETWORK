#include "MainGame.h"
#include "Macro.h"
#include "..\..\Common\PACKET_HEADER.h"
#include "UserInfo.h"
#include "Player.h"
#include "BitMap.h"
#include "ResourceManager.h"
#include "ChessBoard.h"
#include "Chat.h"
#include "SceneManager.h"

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
	m_bReady = false;

	m_chessBoard->Init();
	m_chat->Init(hWnd, hInst);
	SendTurn();
}

void MainGame::Update(float fElapseTime)
{
	m_chat->Update();

	if (m_bReady)
	{
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

		if (packet.deadChessPiece == 6 || packet.deadChessPiece == 12)
		{
			SendGameEnd();
		}
	}
	break;

	case PACKET_INDEX_SEND_TURN_INIT:
	{
		PACKET_SEND_TURN_INIT packet;
		memcpy(&packet, szBuf, header.wLen);

		USER_INFO->m_color = packet.color;
		USER_INFO->m_mapPlayer[USER_INFO->m_userIndex]->turn = packet.data.turn;

		if (USER_INFO->m_color == 1)
			SendReady();
	}
	break;

	case PACKET_INDEX_SEND_READY:
	{
		PACKET_SEND_READY packet;
		memcpy(&packet, szBuf, header.wLen);

		m_bReady = true;
	}
	break;

	case PACKET_INDEX_SEND_GAME_END:
	{
		PACKET_SEND_GAME_END packet;
		memcpy(&packet, szBuf, header.wLen);

		USER_INFO->m_mapPlayer[USER_INFO->m_userIndex]->roomNumber = 99;
		SceneManager::GetInstance()->ChangeScene(LOBBY);
	}
	break;

	}
}

void MainGame::SendTurn()
{
	PACKET_SEND_TURN_INIT packet;
	packet.header.wIndex = PACKET_INDEX_SEND_TURN_INIT;
	packet.header.wLen = sizeof(packet);
	//Èì
	packet.data.iIndex = USER_INFO->m_mapPlayer[USER_INFO->m_userIndex]->index;
	packet.data.roomNumber = USER_INFO->m_mapPlayer[USER_INFO->m_userIndex]->roomNumber;

	send(USER_INFO->m_socket, (const char*)&packet, sizeof(packet), 0);
}

void MainGame::SendReady()
{
	PACKET_SEND_READY packet;
	packet.header.wIndex = PACKET_INDEX_SEND_READY;
	packet.header.wLen = sizeof(packet);
	
	packet.roomNumber = USER_INFO->m_mapPlayer[USER_INFO->m_userIndex]->roomNumber;

	send(USER_INFO->m_socket, (const char*)&packet, sizeof(packet), 0);
}

void MainGame::SendGameEnd()
{
	PACKET_SEND_GAME_END packet;
	packet.header.wIndex = PACKET_INDEX_SEND_GAME_END;
	packet.header.wLen = sizeof(packet);

	send(USER_INFO->m_socket, (const char*)&packet, sizeof(packet), 0);
}