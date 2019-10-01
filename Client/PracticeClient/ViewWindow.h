#pragma once
#include <Windows.h>
#include <vector>
#include <string>

using namespace std;

class BitMap;
class ViewWindow
{
private:
	BitMap*			m_pViewWindow;
	vector<int>		m_vecGuest;
public:
	void Init();
	void UpdateLobbyView();
	void UpdateRoomView();
	void Render();
public:

public:
	ViewWindow();
	~ViewWindow();
};