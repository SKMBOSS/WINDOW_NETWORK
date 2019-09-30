#include "Room.h"
#include "BitMap.h"
#include "ResourceManager.h"
#include "UserInfo.h"
#include "Player.h"
#include "..\..\Common\PACKET_HEADER.h"

int Room::m_sRoomNumber = 0;

Room::Room()
{
}

Room::~Room()
{
}

void Room::Init()
{
	InitRoom();
}

void Room::Render()
{
	m_pRoomBitMap->Render(m_pos.x, m_pos.y);

	m_pRoomBitMap->RenderText(m_pos.x, m_pos.y, to_string(m_iThisNumber+1) + "번 방");

	if(m_userNumCount==100)
		m_pRoomBitMap->RenderText(m_pos.x+10, m_pos.y+25, "Wait Loding..");
	else if (m_userNumCount == 0)
		m_pRoomBitMap->RenderText(m_pos.x+5, m_pos.y+25, "(0/2) WAITING");
	else if (m_userNumCount == 1)
		m_pRoomBitMap->RenderText(m_pos.x + 5, m_pos.y + 25, "(1/2) WAITING");
	else
		m_pRoomBitMap->RenderText(m_pos.x + 5, m_pos.y + 25, "(2/2) PLAYING");
}

void Room::Update(int x, int y)
{
	SelectRoom(x, y);
}

void Room::InitRoom()
{
	m_iThisNumber = m_sRoomNumber++;
	if (m_sRoomNumber == 16)
		m_sRoomNumber = 0;
	m_eRoomState = WAIT;
	m_userNumCount = 100;
	m_pRoomBitMap = nullptr;

	if ((m_iThisNumber >= 0 && m_iThisNumber < 8)
		|| (m_iThisNumber >= 16 && m_iThisNumber < 24)
		|| (m_iThisNumber >= 32 && m_iThisNumber < 40)
		|| (m_iThisNumber >= 48 && m_iThisNumber < 56))
	{
		if (m_iThisNumber % 2 == 0)
			m_pRoomBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BOARD_WHITE);
		else
			m_pRoomBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BOARD_BLACK);
	}
	else
	{
		if (m_iThisNumber % 2 == 0)
			m_pRoomBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BOARD_BLACK);
		else
			m_pRoomBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BOARD_WHITE);
	}

	m_pos.x = ((m_iThisNumber % 8) * m_pRoomBitMap->GetSize().cx);
	m_pos.y = ((m_iThisNumber / 8) * m_pRoomBitMap->GetSize().cy);
}

void Room::SelectRoom(int x, int y)
{
	RECT rc = { m_pos.x , m_pos.y, m_pos.x + m_pRoomBitMap->GetSize().cx, m_pos.y + m_pRoomBitMap->GetSize().cy };
	POINT pt = { x, y };

	if (PtInRect(&rc, pt))
	{
		if (m_userNumCount < 2)
		{
			//여기수정해야함
			PACKET_SEND_CAHNGE_ROOM packet;
			packet.header.wIndex = PACKET_INDEX_SEND_CAHNGE_ROOM;
			packet.header.wLen = sizeof(packet);

			packet.index = USER_INFO->m_userIndex;
			packet.roomNumber = m_iThisNumber;
			send(USER_INFO->m_socket, (const char*)&packet, sizeof(packet), 0);
		}
		else
		{
			//꽉찻어요 메시지 보내기
		}
	}
}

void Room::SetUserNumCount(int count)
{
	m_userNumCount = count;
}