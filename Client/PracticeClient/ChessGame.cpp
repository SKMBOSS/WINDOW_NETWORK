#include "ChessGame.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "..\..\Common\PACKET_HEADER.h"
#include "UserInfo.h"
#include "Player.h"

ChessGame::ChessGame()
{
	m_LastTime = std::chrono::system_clock::now();
}

ChessGame::~ChessGame()
{
}

void ChessGame::Init(HWND hWnd, HINSTANCE hInst,SOCKET sock)
{
	USER_INFO->m_socket = sock;
	m_hWnd = hWnd;
	m_hInst = hInst;
	HDC hdc = GetDC(hWnd);
	m_hMemDC = CreateCompatibleDC(hdc);
	m_hBitmap = CreateCompatibleBitmap(hdc, 900, 900);
	m_hOld = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	ResourceManager::GetInstance()->Init(m_hMemDC);
	SceneManager::GetInstance()->Init(hWnd, hInst);
	ReleaseDC(hWnd, hdc);
	myLen = 0;

}

void ChessGame::Update()
{
	std::chrono::duration<float> sec = std::chrono::system_clock::now() - m_LastTime;
	m_fElapseTime = sec.count();
	m_LastTime = std::chrono::system_clock::now();

	SceneManager::GetInstance()->Update(m_fElapseTime);
}

void ChessGame::Render()
{
	HDC hdc = GetDC(m_hWnd);
	SceneManager::GetInstance()->Render();
	BitBlt(hdc, 0, 0, 900, 900, m_hMemDC, 0, 0, SRCCOPY);
	ReleaseDC(m_hWnd, hdc);
}

void ChessGame::Release()
{
	SceneManager::GetInstance()->Release();
	ResourceManager::GetInstance()->Release();
	
	SelectObject(m_hMemDC, m_hOld);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
	
	for (auto iter = USER_INFO->m_mapPlayer.begin(); iter != USER_INFO->m_mapPlayer.end(); iter++)
	{
		delete iter->second;
	}
	USER_INFO->m_mapPlayer.clear();
}

void ChessGame::ProcessPacket(char * szBuf, int len)
{
	memcpy(&packetBuf[myLen], szBuf, len);
	myLen += len;

	ProcessPacketBuf();

}

void ChessGame::ProcessPacketBuf()
{

	//ÂªÀ½
	if (myLen < sizeof(PACKET_HEADER))
	{
		return;
	}

	PACKET_HEADER header;
	memcpy(&header, packetBuf, sizeof(header));

	if (myLen < header.wLen)
	{
		return;
	}

	SceneManager::GetInstance()->ProcessPacket(packetBuf, header.wLen);

	switch (header.wIndex)
	{
	case PACKET_INDEX_LOGIN_RET:
	{
		PACKET_LOGIN_RET packet;
		memcpy(&packet, packetBuf, header.wLen);
		USER_INFO->m_userIndex = packet.iIndex;
	}
	break;

	case PACKET_INDEX_USER_DATA:
	{
		PACKET_USER_DATA packet;

		memcpy(&packet, packetBuf, header.wLen);
		for (auto iter = USER_INFO->m_mapPlayer.begin(); iter != USER_INFO->m_mapPlayer.end(); iter++)
		{
			delete iter->second;
		}
		USER_INFO->m_mapPlayer.clear();

		for (int i = 0; i < packet.wCount; i++)
		{
			Player* pNew = new Player();

			pNew->index = packet.data[i].iIndex;
			pNew->roomNumber = packet.data[i].roomNumber;
			pNew->turn = packet.data[i].turn;
			USER_INFO->m_mapPlayer.insert(make_pair(packet.data[i].iIndex, pNew));
		}
	}
	break;

	case PACKET_INDEX_SEND_DELETE_USER:
	{
		PACKET_SEND_DELETE_USER packet;
		memcpy(&packet, packetBuf, header.wLen);

		USER_INFO->m_mapPlayer.erase(packet.index);
	}
	break;
	}

	memcpy(&packetBuf, &packetBuf[header.wLen], myLen - header.wLen);
	myLen -= header.wLen;


	if (myLen >= sizeof(PACKET_HEADER))
	{
		ProcessPacketBuf();
	}

}


