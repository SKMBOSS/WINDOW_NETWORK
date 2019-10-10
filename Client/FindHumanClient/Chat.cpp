#include "Chat.h"
#include "BitMap.h"
#include "ResourceManager.h"
#include "..\..\Common\FIND_HUMAN_PACKET_HEADER.h"
#include "UserInfo.h"

Chat::Chat()
{
}

Chat::~Chat()
{
}

void Chat::Init(HWND hWnd, HINSTANCE hInst, int maxRenderText)
{
	m_hWnd = hWnd;
	m_hInst = hInst;
	m_clearMsg = "";
	m_bKeyFirst = false;
	m_maxRenderText = maxRenderText;

	m_hChatWnd = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		ES_AUTOHSCROLL, 12, 875, 788, 25, hWnd, (HMENU)ID_EDIT, hInst, NULL);
}

void Chat::Update()
{
	//≈∞¥≠∏≤
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (m_bKeyFirst == false)
		{
			char getMsg[128];
			GetDlgItemText(m_hWnd, ID_EDIT, getMsg, 128);
			SendChatMsg(getMsg, lstrlen(getMsg));
			SetDlgItemText(m_hWnd, ID_EDIT, m_clearMsg.c_str());

			m_bKeyFirst = true;
		}
	}
	else
	{
		m_bKeyFirst = false;
	}
}

void Chat::Render()
{
	BitMap* text = ResourceManager::GetInstance()->GetBitMap(RES_CHAT_BAR);
	int msgPosY = 850;

	text->Render(790, 300, 10, 575);
	for (auto iter = m_deqRendrMsg.begin(); iter != m_deqRendrMsg.end(); ++iter)
	{
		text->RenderText(10, msgPosY, (*iter).data());
		msgPosY -= 15;
	}
}

void Chat::Relaese()
{

}

void Chat::SendChatMsg(char* msg, int len)
{
	PACKET_SEND_CHAT packet;
	packet.header.wIndex = PACKET_INDEX_SEND_CHAT;
	packet.header.wLen = sizeof(packet);
	//»Ï

	for (int i = 0; i < len; i++)
		packet.szBuf[i] = msg[i];

	packet.len = len;

	send(USER_INFO->m_socket, (const char*)&packet, sizeof(packet), 0);
}


void Chat::ProcessPacket(char* szBuf, int len)
{
	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_SEND_CHAT:
	{
		PACKET_SEND_CHAT packet;
		memcpy(&packet, szBuf, header.wLen);

		string recvMsg;

		for (int i = 0; i < packet.nameLen; i++)
			recvMsg += packet.name[i];

		recvMsg += " : ";

		for (int i = 0; i < packet.len; i++)
			recvMsg += packet.szBuf[i];

		m_deqRendrMsg.push_front(recvMsg);
		if (m_deqRendrMsg.size() == m_maxRenderText)
		{
			m_deqRendrMsg.pop_back();
		}
	}
	break;
	}
}

