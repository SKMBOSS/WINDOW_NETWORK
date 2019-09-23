#include "SceneManager.h"
#include "Scene.h"
#include "MainGame.h"
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
	sceneContainer.insert(make_pair(SCENE_STATE::MAINGAME, new MainGame));
}

void SceneManager::ChangeScene(SCENE_STATE state)
{
	if (currentSceneState == state)
		return;

	GetCurrentScene()->Release();
	currentSceneState = state;
	GetCurrentScene()->Init(m_hWnd);
}

void SceneManager::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	InitSceneContainer();
	currentSceneState = SCENE_STATE::MAINGAME;
	GetCurrentScene()->Init(hWnd);
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