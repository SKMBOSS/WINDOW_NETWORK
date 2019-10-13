#pragma once
#include "Scene.h"
#include <vector>
using namespace std;

class BackGround;
class Chat;
class UserView;
class LobbyRoom;
class Lobby :
	public Scene
{
private:
	HWND				m_hWnd;
	HINSTANCE			m_hInst;

	BackGround*			m_pBackGround;
	UserView*			m_pUserView;
	Chat*				m_pChat;

	vector<LobbyRoom*>	m_vecRoom;
	bool				m_bKeyFirst;
public:
	virtual void Init(HWND hWnd, HINSTANCE hInst);
	virtual void Update(float fElapseTime);
	virtual void Render();
	virtual void Release();
	virtual void ProcessPacket(char* szBuf, int len);
public:
	void SendUserViewUpdate();
	void SendLobbyRoomUpdate();
public:
	Lobby();
	~Lobby();
};

