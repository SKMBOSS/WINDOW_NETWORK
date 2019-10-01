#pragma once
#include "Scene.h"
#include <vector>
using namespace std;

class Room;
class RoomChat;
class ViewWindow;
class Lobby :
	public Scene
{
private:
	HWND			m_hWnd;
	HINSTANCE		m_hInst;

	vector<Room*>	m_vecRoom;
	RoomChat*		m_chat;
	bool			m_bKeyFirst;

	ViewWindow*		m_pViewWindow;
public:
	virtual void Init(HWND hWnd, HINSTANCE hInst);
	virtual void Update(float fElapseTime);
	virtual void Render();
	virtual void Release();
	virtual void ProcessPacket(char* szBuf, int len);
public:
	void SendUpdaeRoom();
	void SendUpdateUser();

public:
	Lobby();
	~Lobby();
};

