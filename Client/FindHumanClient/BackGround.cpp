#include "BackGround.h"
#include "BitMap.h"
#include "ResourceManager.h"

BackGround::BackGround()
{
}

BackGround::~BackGround()
{
}

void BackGround::Init()
{
	m_pBackGroundBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BACKGROUND);
	m_pos.x = 0;
	m_pos.y = 0;
}

void BackGround::Render()
{
	m_pBackGroundBitMap->Render(m_pos.x, m_pos.y);
}
