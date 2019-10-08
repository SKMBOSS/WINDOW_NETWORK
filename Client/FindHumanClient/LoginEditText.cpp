#include "LoginEditText.h"
#include "BitMap.h"
#include "ResourceManager.h"
#include "..\..\Common\FIND_HUMAN_PACKET_HEADER.h"
#include "UserInfo.h"
#include "Player.h"

LoginEditText::LoginEditText()
{
}

LoginEditText::~LoginEditText()
{
}

void LoginEditText::Init(HWND hWnd, HINSTANCE hInst)
{
	m_hWnd = hWnd;
	m_hInst = hInst;
	m_bKeyFirst = false;

	m_hLoginEditTextWnd = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		ES_AUTOHSCROLL, 465, 800, 200, 25, hWnd, (HMENU)ID_EDIT, hInst, NULL);
}

void LoginEditText::Update()
{
	LimitText(12);

	//≈∞¥≠∏≤
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (m_bKeyFirst == false)
		{
			char getMsg[128];
			GetDlgItemText(m_hWnd, ID_EDIT, getMsg, 128);
			SendUserName(getMsg, lstrlen(getMsg));

			m_bKeyFirst = true;
		}
	}
	else
	{
		m_bKeyFirst = false;
	}
}

void LoginEditText::Render()
{
}

void LoginEditText::Relaese()
{}


void LoginEditText::SendUserName(char* msg, int len)
{
	PACKET_SEND_USERNAME packet;
	packet.header.wIndex = PACKET_INDEX_SEND_USERNAME;
	packet.header.wLen = sizeof(packet);
	//»Ï

	for (int i = 0; i < len; i++)
		packet.name[i] = msg[i];

	packet.userIndex = USER_INFO->m_userIndex;
	packet.nameLen = len;
	send(USER_INFO->m_socket, (const char*)&packet, sizeof(packet), 0);
}


void LoginEditText::LimitText(int limitIndex)
{
	char getMsg[128];
	GetDlgItemText(m_hWnd, ID_EDIT, getMsg, 128);

	if (lstrlen(getMsg) > limitIndex)
	{
		char copyMsg[128];

		for (int i = 0; i < limitIndex; i++)
		{
			copyMsg[i] = getMsg[i];
		}
		copyMsg[limitIndex] = '\0';

		SetDlgItemText(m_hWnd, ID_EDIT, copyMsg);
	}
}
