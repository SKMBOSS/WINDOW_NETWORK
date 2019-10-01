#include "UserInfo.h"
#include "Macro.h"
#include "Player.h"

UserInfo::UserInfo()
{
}

UserInfo::~UserInfo()
{
}

UserInfo* UserInfo::m_sThis = nullptr;

UserInfo* UserInfo::GetInstance()
{
	if (m_sThis == nullptr)
	{
		m_sThis = new UserInfo;
	}
	return m_sThis;
}

void UserInfo::Release()
{
	for (auto iter = m_mapPlayer.begin(); iter != m_mapPlayer.end(); iter++)
	{
		delete iter->second;
	}
	m_mapPlayer.clear();

	SAFE_DELETE(m_sThis);
}