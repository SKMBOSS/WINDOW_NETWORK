#pragma once
#include <Windows.h>

class NetworkManager
{
private:
	static NetworkManager* m_sThis;
	SOCKET		m_socket;
public:
	static NetworkManager* GetInstance();
	SOCKET GetSocket();
	void SetSocket(SOCKET sock);
	void Release();
private:
	NetworkManager();
public:
	~NetworkManager();
};

