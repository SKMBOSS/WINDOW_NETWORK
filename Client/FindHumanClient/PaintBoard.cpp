#include "PaintBoard.h"
#include "Macro.h"
#include "BitMap.h"
#include "ResourceManager.h"
#include "PIXEL.h"
#include "..\..\Common\FIND_HUMAN_PACKET_HEADER.h"
#include "UserInfo.h"

PaintBoard::PaintBoard()
{
}


PaintBoard::~PaintBoard()
{
}

void PaintBoard::Init()
{
	m_hdc = ResourceManager::GetInstance()->GetBackBufHdc();

	m_pPaintBoard = ResourceManager::GetInstance()->GetBitMap(RES_PAINT_BOARD);
	m_PaintBoardPos.x = 20;
	m_PaintBoardPos.y = 60;

	m_pPalette = ResourceManager::GetInstance()->GetBitMap(RES_PALETTE);
	m_paletePos.x = 20;
	m_paletePos.y = 570;

	m_pPixcel = ResourceManager::GetInstance()->GetBitMap(RES_PIXEL_SIZE_01);
	m_pixcelPos.x = 500;
	m_pixcelPos.y = 570;

	m_oldPixelPos.x = -1;
	m_oldPixelPos.y = -1;

	m_color = RGB(0, 0, 0);
	m_pixelWidth = 9;
	m_ePaintBoardState = PB_WAITING;
}

void PaintBoard::Update(HWND hWnd)
{
	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{//435 39
		POINT pt;
		RECT rcPaintBoard = { 20 , 60 , 20+760  , 60 + 500 };
		RECT rcPalette = { 20 ,570, 20 + 435, 570 +39 };
		RECT rcPenWidthBoxAndAllDleteBox = { 500 ,570, 500 + 280, 570 + 40 };
		GetCursorPos(&pt);
		ScreenToClient(hWnd, &pt);

		if (PtInRect(&rcPaintBoard, pt))
		{
			if (pt.x == m_oldPixelPos.x && pt.y == m_oldPixelPos.y)
				return;

			if (m_ePaintBoardState == PB_WAITING)
			{
				m_oldPixelPos = pt;
				m_ePaintBoardState = PB_PAINTING;
				SendPixelPos(pt, m_oldPixelPos, m_pixelWidth, m_color);
			}
			else if (m_ePaintBoardState == PB_PAINTING)
			{
				SendPixelPos(pt, m_oldPixelPos, m_pixelWidth, m_color);
				m_oldPixelPos = pt;
			}
		}

		else if (PtInRect(&rcPalette, pt))
		{
			m_color = GetPixel(m_hdc, pt.x, pt.y);
		}

		else if (PtInRect(&rcPenWidthBoxAndAllDleteBox, pt))
		{
			RECT box;
			box.top = rcPenWidthBoxAndAllDleteBox.top;
			box.bottom = box.top + 40;
			box.left = rcPenWidthBoxAndAllDleteBox.left;
			box.right = box.left + 40;

			for (int i = 0; i < 7; i++)
			{
				if (PtInRect(&box, pt))
				{
					switch (i)
					{
					case 0:
						m_pixelWidth = 5;
						break;
					case 1:
						m_pixelWidth = 9;
						break;
					case 2:
						m_pixelWidth = 12;
						break;
					case 3:
						m_pixelWidth = 17;
						break;
					case 4:
						m_pixelWidth = 23;
						break;
					case 5:
						m_pixelWidth = 34;
						break;
					case 6:
						SendAllDelete();
						break;
					}
				}
				if (i != 6)
				{
					box.left += 40;
					box.right += 40;
				}
			}
		}
	}
	else
	{
		if (m_ePaintBoardState == PB_PAINTING)
		{
			m_ePaintBoardState = PB_WAITING;
			m_oldPixelPos.x = -1;
			m_oldPixelPos.y = -1;
		}
	}
}

void PaintBoard::Render()
{
	m_pPaintBoard->Render(760, 500, m_PaintBoardPos.x, m_PaintBoardPos.y);
	m_pPalette->Render(m_pPalette->GetSize().cx, m_pPalette->GetSize().cy, m_paletePos.x, m_paletePos.y);
	m_pPixcel->Render(m_pPixcel->GetSize().cx, m_pPixcel->GetSize().cy, m_pixcelPos.x, m_pixcelPos.y);
	for (auto iter = m_vecPIXEL.begin(); iter != m_vecPIXEL.end(); ++iter)
	{
		m_hPen = CreatePen(PS_SOLID, (*iter)->m_pixelWidth, (*iter)->m_color);
		m_hOldPen = (HPEN)SelectObject(m_hdc, m_hPen);

		if (((*iter)->m_newPos.x == (*iter)->m_oldPos.x) &&
			((*iter)->m_newPos.y == (*iter)->m_oldPos.y))
		{
			SetPixel
			(m_hdc,
				(*iter)->m_newPos.x,
				(*iter)->m_newPos.y,
				RGB(0, 0, 0)
			);
		}
		MoveToEx(m_hdc, (*iter)->m_newPos.x, (*iter)->m_newPos.y, NULL);
		LineTo(m_hdc, (*iter)->m_oldPos.x, (*iter)->m_oldPos.y);

		SelectObject(m_hdc, m_hOldPen);
		DeleteObject(m_hPen);
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

void  PaintBoard::ProcessPacket(char* szBuf, int len)
{
	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_SEND_PIXEL_POS:
	{
		PACKET_SEND_PIXEL_POS packet;
		memcpy(&packet, szBuf, header.wLen);

		MakeAndPushPixel(packet.newPos, packet.oldPos, packet.pixelWidth, packet.color);
	}
	break;
	case PACKET_INDEX_SEND_ALL_DELETE:
	{
		PACKET_SEND_PIXEL_POS packet;
		memcpy(&packet, szBuf, header.wLen);

		Release();
	}
	break;
	}
}

void PaintBoard::SendPixelPos(POINT newPos, POINT oldPos, int pixelWidth, COLORREF color)
{
	PACKET_SEND_PIXEL_POS packet;
	packet.header.wIndex = PACKET_INDEX_SEND_PIXEL_POS;
	packet.header.wLen = sizeof(packet);

	packet.newPos = newPos;
	packet.oldPos = oldPos;

	packet.pixelWidth = pixelWidth;
	packet.color = color;

	send(USER_INFO->m_socket, (const char*)&packet, packet.header.wLen, 0);
}

void PaintBoard::SendAllDelete()
{
	PACKET_SEND_ALL_DELETE packet;
	packet.header.wIndex = PACKET_INDEX_SEND_ALL_DELETE;
	packet.header.wLen = sizeof(packet);

	send(USER_INFO->m_socket, (const char*)&packet, packet.header.wLen, 0);
}

void PaintBoard::MakeAndPushPixel(POINT newPos, POINT oldPos, int pixelWidth, COLORREF color)
{
	PIXEL* newPixel = new PIXEL;
	newPixel->SetPixel(newPos, oldPos, pixelWidth, color);

	m_vecPIXEL.push_back(newPixel);
}