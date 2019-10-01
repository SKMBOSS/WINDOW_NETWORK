#include "ViewWindow.h"
#include "BitMap.h"
#include "ResourceManager.h"
#include "..\..\Common\PACKET_HEADER.h"
#include "UserInfo.h"
#include "Player.h"

ViewWindow::ViewWindow()
{
}

ViewWindow::~ViewWindow()
{
}

void ViewWindow::Init()
{
	m_pViewWindow = ResourceManager::GetInstance()->GetBitMap(RES_VIEW_WINDOW);
}

void ViewWindow::UpdateLobbyView()
{
	m_vecGuest.clear();
	for (auto iter = USER_INFO->m_mapPlayer.begin(); iter != USER_INFO->m_mapPlayer.end(); ++iter)
	{
		if (iter->second->roomNumber == 99)
		{
			m_vecGuest.push_back(iter->second->index);
		}
	}
}

void ViewWindow::UpdateRoomView()
{
	m_vecGuest.clear();
	for (auto iter = USER_INFO->m_mapPlayer.begin(); iter != USER_INFO->m_mapPlayer.end(); ++iter)
	{
		if (iter->second->roomNumber == USER_INFO->m_mapPlayer[USER_INFO->m_userIndex]->roomNumber)
		{
			m_vecGuest.push_back(iter->second->index);
		}
	}
}

void ViewWindow::Render()
{
	m_pViewWindow->Render(800, 0);
	m_pViewWindow->RenderText(800, 0, "<접속자 목록>");

	int posY = 20;
	for (auto iter = m_vecGuest.begin(); iter != m_vecGuest.end(); ++iter)
	{
		m_pViewWindow->RenderText(800, posY, (to_string)(*iter) + "번 GUEST");
		posY += 20;
	}
	
}
