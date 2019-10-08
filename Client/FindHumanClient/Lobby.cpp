#include "Lobby.h"
#include "Macro.h"
#include "SceneManager.h"


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
}

void Lobby::Update(float fElapseTime)
{
}

void Lobby::Render()
{
}

void Lobby::Release()
{

}


void Lobby::ProcessPacket(char* szBuf, int len)
{
}