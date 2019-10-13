#pragma once
#include <Windows.h>
#include <map>
using namespace std;

#define SOCKET_INFO SocketManager::GetInstance()

class SocketManager
{
private:
	static SocketManager* m_sThis;
public:
	static SocketManager* GetInstance();
	SOCKET	m_socket;
public:
	void Release();
private:
	SocketManager();
public:
	~SocketManager();
};

