#pragma once
#include <Windows.h>
#include <string>

using namespace std;

class BitMap;
class UserReadyBox
{
private:
	HDC		m_hdc;
	int		m_userNum;
	bool	m_bArruserReday[8];
public:
	void Init();
	void Render();
	void ProcessPacket(char* szBuf, int len);
public:
	UserReadyBox();
	~UserReadyBox();
};