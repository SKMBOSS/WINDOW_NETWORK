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
	string chess = basePath + "chess\\";
	string chat = basePath + "chat\\";

	//보드
	resPath.push_back(chess + "block00.bmp");
	resPath.push_back(chess + "block01.bmp");
	resPath.push_back(chess + "block02.bmp");
	//백
	resPath.push_back(chess + "block_w_00.bmp");
	resPath.push_back(chess + "block_w_01.bmp");
	resPath.push_back(chess + "block_w_02.bmp");
	resPath.push_back(chess + "block_w_03.bmp");
	resPath.push_back(chess + "block_w_04.bmp");
	resPath.push_back(chess + "block_w_05.bmp");
	//흑
	resPath.push_back(chess + "block_b_00.bmp");
	resPath.push_back(chess + "block_b_01.bmp");
	resPath.push_back(chess + "block_b_02.bmp");
	resPath.push_back(chess + "block_b_03.bmp");
	resPath.push_back(chess + "block_b_04.bmp");
	resPath.push_back(chess + "block_b_05.bmp");
	//텍스트용
	resPath.push_back(chat + "text_window.bmp");
	resPath.push_back(chat + "text_window_lobby.bmp");
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