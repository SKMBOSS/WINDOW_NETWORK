#pragma once
#include <Windows.h>
#include <string>
#define ID_EDIT 100

using namespace std;

class LoginEditText
{
private:
	HWND			m_hWnd;
	HINSTANCE		m_hInst;

	HWND			m_hLoginEditTextWnd;
	bool		    m_bKeyFirst;
public:
	void Init(HWND hWnd, HINSTANCE hInst);
	void Update();
	void Render();
	void Relaese();
public:
	void LimitText(int limitIndex);
	void SendUserName(char *msg, int len);
public:
	LoginEditText();
	~LoginEditText();
};

