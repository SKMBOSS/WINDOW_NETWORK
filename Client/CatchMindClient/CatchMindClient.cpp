#include "CatchMindClient.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "..\..\Common\CATCH_MIND_PACKET_HEADER.h"
#include "SocketManager.h"
#include "Macro.h"

CatchMindClient* CatchMindClient::m_sThis = nullptr;

CatchMindClient::CatchMindClient()
{
	m_LastTime = std::chrono::system_clock::now();
}

CatchMindClient::~CatchMindClient()
{
}

CatchMindClient* CatchMindClient::GetInstance()
{
	if (m_sThis == nullptr)
	{
		m_sThis = new CatchMindClient;
	}
	return m_sThis;
}

void CatchMindClient::Init(HWND hWnd, HINSTANCE hInst, SOCKET sock)
{
	SOCKET_INFO->m_socket = sock;
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

void CatchMindClient::Update()
{
	std::chrono::duration<float> sec = std::chrono::system_clock::now() - m_LastTime;
	m_fElapseTime = sec.count();
	m_LastTime = std::chrono::system_clock::now();

	SceneManager::GetInstance()->Update(m_fElapseTime);
}

void CatchMindClient::Render()
{
	HDC hdc = GetDC(m_hWnd);
	SceneManager::GetInstance()->Render();
	BitBlt(hdc, 0, 0, 900, 900, m_hMemDC, 0, 0, SRCCOPY);
	ReleaseDC(m_hWnd, hdc);
}

void CatchMindClient::Release()
{
	SOCKET_INFO->Release();
	SceneManager::GetInstance()->Release();
	ResourceManager::GetInstance()->Release();

	SelectObject(m_hMemDC, m_hOld);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);

	SAFE_DELETE(m_sThis);
}

void CatchMindClient::ProcessPacket(char * szBuf, int len)
{
	memcpy(&packetBuf[myLen], szBuf, len);
	myLen += len;

	ProcessPacketBuf();

}

void CatchMindClient::ProcessPacketBuf()
{
	//ª�
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
	case PACKET_INDEX_SEND_USERNAME:
	{
		PACKET_SEND_USERNAME namePacket;
		memcpy(&namePacket, packetBuf, header.wLen);

		SendLoginOk();
	}
	}

	memcpy(&packetBuf, &packetBuf[header.wLen], myLen - header.wLen);
	myLen -= header.wLen;

	if (myLen >= sizeof(PACKET_HEADER))
	{
		ProcessPacketBuf();
	}

}

void CatchMindClient::SendLoginOk()
{
	PACKET_SEND_LOGIN_OK packet;
	packet.header.wIndex = PACKET_INDEX_SEND_LOGIN_OK;
	packet.header.wLen = sizeof(packet);

	send(SOCKET_INFO->m_socket, (const char*)&packet, sizeof(packet), 0);
}


