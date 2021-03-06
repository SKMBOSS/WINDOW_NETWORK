#pragma once
#include "Scene.h"

class LoginEditText;
class BackGround;
class Logo;
class Login :
	public Scene
{
private:
	HWND			m_hWnd;
	HINSTANCE		m_hInst;
	BackGround*		m_pBackGround;
	Logo*			m_pLogo;
	LoginEditText*	m_pLoginEditText;

public:
	virtual void Init(HWND hWnd, HINSTANCE hInst);
	virtual void Update(float fElapseTime);
	virtual void Render();
	virtual void Release();
	virtual void ProcessPacket(char* szBuf, int len);
public:
	Login();
	virtual ~Login();
};

