#pragma once
#include <Windows.h>
#include <map>
using namespace std;

#define USER_INFO UserInfo::GetInstance()

class Player;
class UserInfo
{
private:
	static UserInfo* m_sThis;
public:
	static UserInfo* GetInstance();
	SOCKET				m_socket;
	map<int, Player*>	m_mapPlayer;
	int					m_userIndex = 0;
	int					m_color=100;

public:
	void Release();
private:
	UserInfo();
public:
	~UserInfo();
};

