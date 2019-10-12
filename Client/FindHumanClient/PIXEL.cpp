#include "PIXEL.h"
#include "BitMap.h"
#include "ResourceManager.h"

PIXEL::PIXEL()
{
}

PIXEL::~PIXEL()
{
}

void PIXEL::Init(int x, int y)
{
	//m_pPixelBitmap = ResourceManager::GetInstance()->GetBitMap(RES_BRUSH_BLACK);
	m_Pos.x = x;
	m_Pos.y = y;
}

void PIXEL::Render()
{
	//m_pPixelBitmap->Render(m_pPixelBitmap->GetSize().cx, m_pPixelBitmap->GetSize().cy, m_pPos.x, m_pPos.y);
}