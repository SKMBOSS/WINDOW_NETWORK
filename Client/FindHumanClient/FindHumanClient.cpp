#include "FindHumanClient.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "..\..\Common\FIND_HUMAN_PACKET_HEADER.h"
#include "UserInfo.h"
#include "Player.h"
#include "Macro.h"

FindHumanClient* FindHumanClient::m_sThis = nullptr;

FindHumanClient::FindHumanClient()
{
	m_LastTime = std::chrono::system_clock::now();
}

FindHumanClient::~FindHumanClient()
{
}

FindHumanClient* FindHumanClient::GetInstance()
{
	if (m_sThis == nullptr)
	{
		m_sThis = new FindHumanClient;
	}
	return m_sThis;
}

void FindHumanClient::Init(HWND hWnd, HINSTANCE hInst, SOCKET sock)
{
	USER_INFO->m_socket = sock;
	m_hWnd = hWnd;
	m_hInst = hInst;
	HDC hdc = GetDC(hWnd);
	m_hMemDC = CreateCompatibleDC(hdc);
	m_hBitmap = CreateCompatibleBitmap(hdc, 1100, 1100);
	m_hOld = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	ResourceManager::GetInstance()->Init(m_hMemDC);
	SceneManager::GetInstance()->Init(hWnd, hInst);
	ReleaseDC(hWnd, hdc);
	myLen = 0;

}

void FindHumanClient::Update()
{
	std::chrono::duration<float> sec = std::chrono::system_clock::now() - m_LastTime;
	m_fElapseTime = sec.count();
	m_LastTime = std::chrono::system_clock::now();

	SceneManager::GetInstance()->Update(m_fElapseTime);
}

void FindHumanClient::Render()
{
	HDC hdc = GetDC(m_hWnd);
	SceneManager::GetInstance()->Render();
	BitBlt(hdc, 0, 0, 1100, 1100, m_hMemDC, 0, 0, SRCCOPY);
	ReleaseDC(m_hWnd, hdc);
}

void FindHumanClient::Release()
{
	USER_INFO->Release();
	SceneManager::GetInstance()->Release();
	ResourceManager::GetInstance()->Release();

	SelectObject(m_hMemDC, m_hOld);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);

	SAFE_DELETE(m_sThis);
}

void FindHumanClient::ProcessPacket(char * szBuf, int len)
{
	memcpy(&packetBuf[myLen], szBuf, len);
	myLen += len;

	ProcessPacketBuf();

}

void FindHumanClient::ProcessPacketBuf()
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
	case PACKET_INDEX_LOGIN:
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
			USER_INFO->m_mapPlayer.insert(make_pair(packet.data[i].iIndex, pNew));
		}
	}
	break;

	case PACKET_INDEX_SEND_USERNAME:
	{
		PACKET_SEND_USERNAME packet;
		memcpy(&packet, packetBuf, header.wLen);

		for (int i = 0; i < packet.nameLen; i++)
		{
			USER_INFO->m_mapPlayer[packet.userIndex]->name[i] = packet.name[i];
		}
		USER_INFO->m_mapPlayer[packet.userIndex]->name[packet.nameLen] = '\0';
		USER_INFO->m_mapPlayer[packet.userIndex]->roomNumber = packet.roomNumber;

		if(USER_INFO ->m_userIndex == packet.userIndex)
		SendLoginOk();
	}

	case PACKET_INDEX_SEND_DELETE_USER:
	{
		PACKET_SEND_DELETE_USER packet;
		memcpy(&packet, packetBuf, header.wLen);

		USER_INFO->m_mapPlayer.erase(packet.index);
	}
	break;

	case PACKET_INDEX_SEND_UPDATE_USER_VIEW:
	{
		PACKET_SEND_UPDATE_USER_VIEW packet;
		memcpy(&packet, packetBuf, header.wLen);

		USER_INFO->m_mapPlayer[packet.index]->roomNumber = packet.roomNumber;
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

void FindHumanClient::SendLoginOk()
{
	PACKET_SEND_LOGIN_OK packet;
	packet.header.wIndex = PACKET_INDEX_SEND_LOGIN_OK;
	packet.header.wLen = sizeof(packet);
	
	send(USER_INFO->m_socket, (const char*)&packet, sizeof(packet), 0);
}


