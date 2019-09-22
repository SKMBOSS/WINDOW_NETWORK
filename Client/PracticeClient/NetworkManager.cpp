#include "NetworkManager.h"
#include "Macro.h"

NetworkManager* NetworkManager::m_sThis = nullptr;


NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{
}

NetworkManager* NetworkManager::GetInstance()
{
	if (m_sThis == nullptr)
		m_sThis = new NetworkManager();

	return m_sThis;
}

void NetworkManager::SetSocket(SOCKET sock)
{
	m_socket = sock;
}

SOCKET NetworkManager::GetSocket()
{
	return m_socket;
}

void NetworkManager::Release()
{
	closesocket(m_socket);
	SAFE_DELETE(m_sThis);
}