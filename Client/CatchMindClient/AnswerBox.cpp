#include "AnswerBox.h"
#include "BitMap.h"
#include "ResourceManager.h"
#include "SocketManager.h"
#include "..\..\Common\CATCH_MIND_PACKET_HEADER.h"

AnswerBox::AnswerBox()
{
}

AnswerBox::~AnswerBox()
{
}

void AnswerBox::Init()
{
	m_pAnswerBoxBitMap = ResourceManager::GetInstance()->GetBitMap(RES_ANSWER_BOX);
	m_pos.x = 550;
	m_pos.y = 20;
}

void AnswerBox::Render()
{
	m_pAnswerBoxBitMap->Render(200,30, m_pos.x, m_pos.y);
	m_pAnswerBoxBitMap->RenderText(m_pos.x + 100, m_pos.y+5, "È£¶ûÀÌ");
}


void AnswerBox::ProcessPacket(char* szBuf, int len)
{
	//
}
