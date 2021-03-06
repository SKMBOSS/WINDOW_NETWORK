#pragma once
#include <Windows.h>
#include <string>

using namespace std;

class BitMap;
class UserView
{
private:
	BitMap*			m_pUserView;
	
	int				m_userNum;
	string			m_userName;
	int				m_stringLen;
public:
	void Init();
	void Render();
	void ProcessPacket(char* szBuf, int len);
public:
	UserView();
	~UserView();
};