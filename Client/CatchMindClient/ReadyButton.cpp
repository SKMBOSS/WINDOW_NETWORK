#include "ReadyButton.h"
#include "BitMap.h"
#include "ResourceManager.h"
#include "SocketManager.h"
#include "..\..\Common\CATCH_MIND_PACKET_HEADER.h"

ReadyButton::ReadyButton()
{
}

ReadyButton::~ReadyButton()
{
}

void ReadyButton::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	m_pReadyButtonBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BUTTON_READY);
	m_pos.x = 20;
	m_pos.y = 10;

	m_bKeyFirst = false;
}

void ReadyButton::Update()
{
	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		if (m_bKeyFirst == false)
		{
			POINT pt;
			RECT rc = { 20 , 10 , 20 + 100, 10 +40 };
			GetCursorPos(&pt);
			ScreenToClient(m_hWnd, &pt);

			if (PtInRect(&rc, pt))
			{
				SendReadyButtonClick();
			}
			m_bKeyFirst = true;
		}
	}
	else
	{
		m_bKeyFirst = false;
	}
}

void ReadyButton::Render()
{
	m_pReadyButtonBitMap->Render(100, 40, m_pos.x, m_pos.y);
}

void ReadyButton::SendReadyButtonClick()
{
	PACKET_SEND_READY_BUTTON_CLICK packet;
	packet.header.wIndex = PACKET_INDEX_SEND_READY_BUTTON_CLICK;
	packet.header.wLen = sizeof(packet);

	send(SOCKET_INFO->m_socket, (const char*)&packet, packet.header.wLen, 0);
}
