#include "Player.h"
#include "..\..\Common\PACKET_HEADER.h"
#include "NetworkManager.h"
Player::Player()
{
}


Player::~Player()
{
}

void Player::SendPos()
{
	int g_iIndex = 0;

	PACKET_SEND_POS packet;
	packet.header.wIndex = PACKET_INDEX_SEND_POS;
	packet.header.wLen = sizeof(packet);
	packet.data.iIndex = g_iIndex;
	packet.data.wX = g_mapPlayer[g_iIndex]->x;
	packet.data.wY = g_mapPlayer[g_iIndex]->y;
	send(NetworkManager::GetInstance()->GetSocket(), (const char*)&packet, sizeof(packet), 0);
	send(NetworkManager::GetInstance()->GetSocket(), (const char*)&packet, sizeof(packet), 0);
}