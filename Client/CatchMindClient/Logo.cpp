#include "Logo.h"
#include "BitMap.h"
#include "ResourceManager.h"

Logo::Logo()
{
}

Logo::~Logo()
{
}

void Logo::Init()
{
	m_LogoBitMap = ResourceManager::GetInstance()->GetBitMap(RES_LOGO);
	m_pos.x = 180;
	m_pos.y = 200;
}

void Logo::Render()
{
	m_LogoBitMap->Render(m_LogoBitMap->GetSize().cx, m_LogoBitMap->GetSize().cy, m_pos.x, m_pos.y);
}