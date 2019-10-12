#include "ResourceManager.h"
#include "BitMap.h"
#include "Macro.h"

ResourceManager* ResourceManager::m_sThis = nullptr;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

ResourceManager* ResourceManager::GetInstance()
{
	if (m_sThis == nullptr)
		m_sThis = new ResourceManager();

	return m_sThis;
}

void ResourceManager::Init(HDC hdc)
{
	InitResPath();
	InitBitMap(hdc);
}

void ResourceManager::Release()
{
	for (int i = 0; i < RES_TYPE_END; i++)
	{
		SAFE_DELETE(m_pBitMap[i]);
	}
	SAFE_DELETE(m_sThis);
}

void ResourceManager::InitResPath()
{
	resPath.reserve(RES_TYPE_END);

	string basePath = "resource\\";
	string backGround = basePath + "backGround\\";
	string chat = basePath + "chat\\";
	string userView = basePath + "userView\\";
	string lobby = basePath + "lobby\\";
	string paintBoard = basePath + "paintBoard\\";

	//backGround
	resPath.push_back(backGround + "backGround.bmp");
	resPath.push_back(chat + "chatBar.bmp");
	resPath.push_back(userView + "userView.bmp");
	resPath.push_back(lobby + "lobbyRoom.bmp");
	resPath.push_back(lobby + "waitng.bmp");
	resPath.push_back(lobby + "full.bmp");
	resPath.push_back(lobby + "playing.bmp");
	resPath.push_back(paintBoard + "paintBoard.bmp");
	resPath.push_back(paintBoard + "blackBrush.bmp");
}

void ResourceManager::InitBitMap(HDC hdc)
{
	for (int i = 0; i < RES_TYPE_END; i++)
	{
		m_pBitMap[i] = new BitMap();
		m_pBitMap[i]->Init(hdc, resPath[i]);
	}
}

BitMap* ResourceManager::GetBitMap(int index)
{
	//인덱스검사해야함
	return m_pBitMap[index];
}

SIZE ResourceManager::GetBitMapSize(int index)
{
	return m_pBitMap[index]->GetSize();
}