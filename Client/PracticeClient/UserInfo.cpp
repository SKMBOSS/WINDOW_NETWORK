#include "UserInfo.h"
#include "Macro.h"

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
	SAFE_DELETE(m_sThis);
}