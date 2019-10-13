#include "SocketManager.h"
#include "Macro.h"

SocketManager::SocketManager()
{
}

SocketManager::~SocketManager()
{
}

SocketManager* SocketManager::m_sThis = nullptr;

SocketManager* SocketManager::GetInstance()
{
	if (m_sThis == nullptr)
	{
		m_sThis = new SocketManager;
	}
	return m_sThis;
}

void SocketManager::Release()
{
	SAFE_DELETE(m_sThis);
}