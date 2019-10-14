#include "UserReadyBox.h"
#include "ResourceManager.h"
#include "..\..\Common\CATCH_MIND_PACKET_HEADER.h"
#include "SocketManager.h"

UserReadyBox::UserReadyBox()
{
}

UserReadyBox::~UserReadyBox()
{
}

void UserReadyBox::Init()
{
	m_hdc = ResourceManager::GetInstance()->GetBackBufHdc();

	m_userNum = 0;

	for (int i = 0; i < 8; i++)
	{
		m_bArruserReday[i] = false;
	}
}

void UserReadyBox::Render()
{
	string text = "----->READY<-----";
	int posY = 40;

	for (int i = 0; i < m_userNum; i++)
	{
		if (m_bArruserReday[i] == true)
		{
			SetBkMode(m_hdc, OPAQUE);
			SetBkColor(m_hdc, RGB(255, 255, 0));
			TextOut(m_hdc, 801, posY, text.c_str(), text.length());
		}
		posY += 20;
	}

}

void UserReadyBox::ProcessPacket(char* szBuf, int len)
{
	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_SEND_USER_READY_BOX:
	{
		PACKET_SEND_USER_READY_BOX packet;
		memcpy(&packet, szBuf, header.wLen);

		m_userNum = packet.userNum;

		for (int i = 0; i < packet.userNum; i++)
		{
			m_bArruserReday[i] = packet.userReady[i];
		}
	}
	break;
	}
}
