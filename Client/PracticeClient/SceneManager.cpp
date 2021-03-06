#include "SceneManager.h"
#include "Scene.h"
#include "MainGame.h"
#include "Lobby.h"
#include "Macro.h"
using namespace std;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

SceneManager* SceneManager::m_sThis = nullptr;

SceneManager* SceneManager::GetInstance()
{
	if (m_sThis == nullptr)
	{
		m_sThis = new SceneManager;
	}
	return m_sThis;
}

void SceneManager::InitSceneContainer()
{
	sceneContainer.insert(make_pair(SCENE_STATE::LOBBY, new Lobby));
	sceneContainer.insert(make_pair(SCENE_STATE::MAINGAME, new MainGame));
}

void SceneManager::ChangeScene(SCENE_STATE state)
{
	if (currentSceneState == state)
		return;

	GetCurrentScene()->Release();
	currentSceneState = state;
	GetCurrentScene()->Init(m_hWnd, m_hInst);
}

void SceneManager::Init(HWND hWnd, HINSTANCE hInst)
{
	m_hWnd = hWnd;
	m_hInst = hInst;
	InitSceneContainer();
	currentSceneState = SCENE_STATE::LOBBY;
	GetCurrentScene()->Init(hWnd, hInst);
}

void SceneManager::Update(float fElapseTime)
{
	GetCurrentScene()->Update(fElapseTime);
}

void SceneManager::Render()
{
	GetCurrentScene()->Render();
}

void SceneManager::Release()
{
	GetCurrentScene()->Release();

	for (auto iter = sceneContainer.begin(); iter != sceneContainer.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	sceneContainer.clear();
	SAFE_DELETE(m_sThis);
}

void SceneManager::ProcessPacket(char* szBuf, int len)
{
	GetCurrentScene()->ProcessPacket(szBuf, len);
}