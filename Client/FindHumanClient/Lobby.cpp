#include "Lobby.h"
#include "Macro.h"
#include "..\..\Common\FIND_HUMAN_PACKET_HEADER.h"
#include "SceneManager.h"
#include "BackGround.h"
#include "Chat.h"


Lobby::Lobby()
{
}


Lobby::~Lobby()
{
}


void Lobby::Init(HWND hWnd, HINSTANCE hInst)
{
	m_hWnd = hWnd;
	m_hInst = hInst;

	m_pBackGround = new BackGround;
	m_pBackGround->Init();

	m_pChat = new Chat;
	m_pChat->Init(hWnd, hInst, 25);
}

void Lobby::Update(float fElapseTime)
{
	m_pChat->Update();
}

void Lobby::Render()
{
	m_pBackGround->Render();
	m_pChat->Render();
}

void Lobby::Release()
{
	SAFE_DELETE(m_pBackGround);
	SAFE_DELETE(m_pChat);
}


void Lobby::ProcessPacket(char* szBuf, int len)
{
	m_pChat->ProcessPacket(szBuf, len);

	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));
}