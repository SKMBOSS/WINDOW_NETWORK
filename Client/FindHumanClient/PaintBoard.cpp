#include "PaintBoard.h"
#include "Macro.h"
#include "BitMap.h"
#include "ResourceManager.h"
#include "PIXEL.h"

PaintBoard::PaintBoard()
{
}


PaintBoard::~PaintBoard()
{
}

void PaintBoard::Init(HDC hdc)
{
	m_hdc = hdc;
	m_pPaintBoard = ResourceManager::GetInstance()->GetBitMap(RES_PAINT_BOARD);
	m_Pos.x = 20;
	m_Pos.y = 60;
}

void PaintBoard::Update(int x, int y)
{
	PIXEL* newPixel = new PIXEL;
	newPixel->Init(x, y);

	m_vecPIXEL.push_back(newPixel);
}

void PaintBoard::Render()
{
	m_pPaintBoard->Render(760, 500, m_Pos.x, m_Pos.y);

	auto startIter = m_vecPIXEL.begin();
	startIter++;

	auto oldIter = m_vecPIXEL.begin();
	HDC hdc = ResourceManager::GetInstance()->GetBackBufHdc();


	if (m_vecPIXEL.size() > 0)
	{
		for (auto iter = startIter; iter != m_vecPIXEL.end(); ++iter)
		{
			//(*iter)->Render();
			MoveToEx(hdc, (*(oldIter))->m_Pos.x, (*(oldIter))->m_Pos.y, NULL);
			LineTo(hdc, (*iter)->m_Pos.x, (*iter)->m_Pos.y);
			oldIter++;
		}
	}
	
}

void PaintBoard::Release()
{
	for (auto iter = m_vecPIXEL.begin(); iter != m_vecPIXEL.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
	m_vecPIXEL.clear();
}