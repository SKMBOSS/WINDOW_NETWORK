#include "StartButton.h"
#include "BitMap.h"
#include "ResourceManager.h"

StartButton::StartButton()
{
}

StartButton::~StartButton()
{
}

void StartButton::Init()
{
	m_pStartButtonBitMap = ResourceManager::GetInstance()->GetBitMap(RES_BUTTON_START);
	m_pos.x = 130;
	m_pos.y = 10;
}

void StartButton::Render()
{
	m_pStartButtonBitMap->Render(100, 40, m_pos.x, m_pos.y);
}
