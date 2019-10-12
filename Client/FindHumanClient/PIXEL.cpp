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
	m_pPixelBitmap = ResourceManager::GetInstance()->GetBitMap(RES_BRUSH_BLACK);
	m_pPos.x = x;
	m_pPos.y = y;
}

void PIXEL::Render()
{
	m_pPixelBitmap->Render(10, 10, m_pPos.x, m_pPos.y);
}