#pragma once
#include <Windows.h>
#include <map>
using namespace std;

#define USER_INFO UserInfo::GetInstance()

class UserInfo
{
private:
	static UserInfo* m_sThis;
public:
	static UserInfo* GetInstance();
	SOCKET	m_socket;
public:
	void Release();
private:
	UserInfo();
public:
	~UserInfo();
};

