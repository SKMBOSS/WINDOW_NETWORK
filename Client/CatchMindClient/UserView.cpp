#include "UserView.h"
#include "BitMap.h"
#include "ResourceManager.h"
#include "..\..\Common\CATCH_MIND_PACKET_HEADER.h"
#include "SocketManager.h"

UserView::UserView()
{
}

UserView::~UserView()
{
}

void UserView::Init()
{
	m_pUserView = ResourceManager::GetInstance()->GetBitMap(RES_USER_VIEW);
	m_userNum = 0;
	m_stringLen = 0;
	m_userName = "";
}

void UserView::Render()
{
	m_pUserView->RenderAlphaBlend(100, 900, 800, 0, 200);
	m_pUserView->RenderText(810, 20, "<Á¢¼ÓÀÚ>");

	int posY = 40;
	int stringIndex = 0;
	string userName;
	for (int i = 0; i < m_userNum; i++)
	{
		while (true)
		{
			userName += m_userName[stringIndex++];
			if (m_userName[stringIndex] == '\0')
			{
				stringIndex++;
				break;
			}
		}
		m_pUserView->RenderText(802, posY, userName);
		userName.clear();
		posY += 20;
	}

}

void UserView::ProcessPacket(char* szBuf, int len)
{
	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_SEND_USER_VIEW:
	{
		PACKET_SEND_USER_VIEW packet;
		memcpy(&packet, szBuf, header.wLen);

		m_userNum = packet.userNum;
		m_stringLen = packet.len;

		m_userName.clear();
		for (int i = 0; i < packet.len; i++)
		{
			m_userName += packet.szBuf[i];
		}
	}
	break;
	}
}
