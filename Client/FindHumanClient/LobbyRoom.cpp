#include "LobbyRoom.h"
#include "BitMap.h"
#include "ResourceManager.h"
#include "UserInfo.h"
#include "..\..\Common\FIND_HUMAN_PACKET_HEADER.h"

int LobbyRoom::m_sRoomNumber = 0;

LobbyRoom::LobbyRoom()
{
}

LobbyRoom::~LobbyRoom()
{
}

void LobbyRoom::Init()
{
	InitRoom();
}

void LobbyRoom::Update(int x, int y)
{
	SelectRoom(x, y);
}

void LobbyRoom::Render()
{
	m_pRoomBitMap->Render(390, 100, m_pos.x+10, m_pos.y);
	m_pRoomBitMap->RenderText(m_pos.x + 136, m_pos.y + 24, (to_string)(m_iThisNumber + 1) + "번 방");
	m_pRoomBitMap->RenderText(m_pos.x + 320, m_pos.y + 60, (to_string)(m_iUserNum) + "/" + (to_string)(MAX_USER));
}

void LobbyRoom::InitRoom()
{
	m_iThisNumber = m_sRoomNumber++;
	if (m_sRoomNumber == 12)
		m_sRoomNumber = 0;

	m_pRoomBitMap = ResourceManager::GetInstance()->GetBitMap(RES_LOBBY_ROOM);

	m_pos.x = ((m_iThisNumber % 2) * 390);

	if (m_iThisNumber < 2)
		m_pos.y = 0;
	else if (m_iThisNumber < 4)
		m_pos.y = 100;
	else if (m_iThisNumber < 6)
		m_pos.y = 200;
	else if (m_iThisNumber < 8)
		m_pos.y = 300;
	else if (m_iThisNumber < 10)
		m_pos.y = 400;
	else 
		m_pos.y = 500;
}

void LobbyRoom::ProcessPacket(char* szBuf, int len)
{
	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));
}

void LobbyRoom::UpdateRoom(int userNum)
{
	m_iUserNum = userNum;
}

void LobbyRoom::SelectRoom(int x, int y)
{
	RECT rc = { m_pos.x , m_pos.y, m_pos.x + m_pRoomBitMap->GetSize().cx, m_pos.y + m_pRoomBitMap->GetSize().cy };
	POINT pt = { x, y };

	if (PtInRect(&rc, pt))
	{
		if (m_iUserNum < 9)
		{
			//여기수정해야함
			PACKET_SEND_CAHNGE_ROOM packet;
			packet.header.wIndex = PACKET_INDEX_SEND_CAHNGE_ROOM;
			packet.header.wLen = sizeof(packet);

			packet.roomNumber = m_iThisNumber+1;
			send(USER_INFO->m_socket, (const char*)&packet, sizeof(packet), 0);
		}
		else
		{
			//꽉찻어요 메시지 보내기
		}
	}
}