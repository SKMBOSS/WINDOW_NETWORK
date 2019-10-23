#pragma once
#include <Windows.h>
#include <string>
using namespace std;

class BitMap;
class AnswerBox
{
private:
	BitMap*		m_pAnswerBoxBitMap;
	POINT		m_pos;
	string		m_answer;

public:
	void Init();
	void Render();
public:
	void ProcessPacket(char* szBuf, int len);
public:
	AnswerBox();
	~AnswerBox();
};