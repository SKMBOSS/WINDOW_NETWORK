#include "Login.h"
#include "..\..\Common\CATCH_MIND_PACKET_HEADER.h"
#include "Macro.h"
#include "SocketManager.h"
#include "BackGround.h"
#include "LoginEditText.h"
#include "SceneManager.h"

Login::Login()
{
}

Login::~Login()
{
}

void Login::Init(HWND hWnd, HINSTANCE hInst)
{
	m_hWnd = hWnd;
	m_hInst = hInst;

	m_pBackGround = new BackGround;
	m_pLoginEditText = new LoginEditText;

	m_pBackGround->Init();
	m_pLoginEditText->Init(m_hWnd, m_hInst);
}

void Login::Update(float fElapseTime)
{
	m_pLoginEditText->Update();
}

void Login::Render()
{
	m_pBackGround->Render();
}

void Login::Release()
{
	SAFE_DELETE(m_pBackGround);

	SAFE_DELETE(m_pLoginEditText);
}


void Login::ProcessPacket(char* szBuf, int len)
{
	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_SEND_LOGIN_OK:
	{
		PACKET_SEND_LOGIN_OK packet;
		memcpy(&packet, szBuf, header.wLen);

		Sleep(500);
		SceneManager::GetInstance()->ChangeScene(LOBBY);

	}
	break;
	}
}