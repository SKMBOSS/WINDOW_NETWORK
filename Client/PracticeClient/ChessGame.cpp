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
	m_hBitmap = CreateCompatibleBitmap(hdc, 800, 900);
	m_hOld = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	ResourceManager::GetInstance()->Init(m_hMemDC);
	SceneManager::GetInstance()->Init(hWnd, hInst);
	ReleaseDC(hWnd, hdc);

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
	BitBlt(hdc, 0, 0, 800, 900, m_hMemDC, 0, 0, SRCCOPY);
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
	SceneManager::GetInstance()->ProcessPacket(szBuf, len);

	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_LOGIN_RET:
	{
		PACKET_LOGIN_RET packet;
		memcpy(&packet, szBuf, header.wLen);
		USER_INFO->m_userIndex = packet.iIndex;
	}
	break;

	case PACKET_INDEX_USER_DATA:
	{
		PACKET_USER_DATA packet;

		memcpy(&packet, szBuf, header.wLen);
		for (auto iter = USER_INFO->m_mapPlayer.begin(); iter != USER_INFO->m_mapPlayer.end(); iter++)
		{
			delete iter->second;
		}
		USER_INFO->m_mapPlayer.clear();

		for (int i = 0; i < packet.wCount; i++)
		{
			Player* pNew = new Player();

			pNew->turn = packet.data[i].turn;
			USER_INFO->m_mapPlayer.insert(make_pair(packet.data[i].iIndex, pNew));
		}
	}
	break;
	}
}
