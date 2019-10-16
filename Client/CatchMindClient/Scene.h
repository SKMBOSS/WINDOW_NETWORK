#pragma once
#include <Windows.h>

class Scene
{
public:
	virtual void Init(HWND hWnd, HINSTANCE hInst) = 0;
	virtual void Update(float fElapseTime) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
	virtual void ProcessPacket(char* szBuf, int len) = 0;
public:
	Scene();
	virtual ~Scene();
};
