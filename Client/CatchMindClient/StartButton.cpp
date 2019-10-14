#include "StartButton.h"
#include "BitMap.h"
#include "ResourceManager.h"
#include "SocketManager.h"
#include "..\..\Common\CATCH_MIND_PACKET_HEADER.h"

StartButton::StartButton()
{
}

StartButton::~StartButton()
{
}

void StartButton::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	m_pStartButtonBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BUTTON_START);
	m_pos.x = 130;
	m_pos.y = 10;

	m_bKeyFirst = false;
}

void StartButton::Update()
{
	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		if (m_bKeyFirst == false)
		{
			POINT pt;
			RECT rc = { 130 , 10 , 130 + 100, 10 + 40 };
			GetCursorPos(&pt);
			ScreenToClient(m_hWnd, &pt);

			if (PtInRect(&rc, pt))
			{
				SendStartButtonClick();
			}
			m_bKeyFirst = true;
		}
	}
	else
	{
		m_bKeyFirst = false;
	}
}

void StartButton::Render()
{
	m_pStartButtonBitMap->Render(100, 40, m_pos.x, m_pos.y);
}

void StartButton::SendStartButtonClick()
{
	PACKET_SEND_START_BUTTON_CLICK packet;
	packet.header.wIndex = PACKET_INDEX_SEND_START_BUTTON_CLICK;
	packet.header.wLen = sizeof(packet);

	send(SOCKET_INFO->m_socket, (const char*)&packet, packet.header.wLen, 0);
}