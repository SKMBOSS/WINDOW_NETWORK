#pragma once
#include <Windows.h>
#include <map>

enum SCENE_STATE
{
	LOBBY,
	MAINGAME
};

class Scene;
class SceneManager
{
private:
	static SceneManager* m_sThis;
	std::map<SCENE_STATE, Scene*> sceneContainer;
	SCENE_STATE currentSceneState;
private:
	HDC		m_hMemDC;
	HWND	m_hWnd;
	HINSTANCE m_hInst;
public:
	static SceneManager* GetInstance();
	inline Scene* GetCurrentScene()
	{
		return sceneContainer[currentSceneState];
	}
	void ChangeScene(SCENE_STATE state);
private:
	void InitSceneContainer();
public:
	void Init(HWND hWnd, HINSTANCE hInst);
	void Update(float fElapseTime);
	void Render();
	void Release();
	void ProcessPacket(char* szBuf, int len);
private:
	SceneManager();
public:
	~SceneManager();
};

