#include "ChessGame.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include <math.h>
#include <stdio.h>

ChessGame::ChessGame()
{
	m_LastTime = std::chrono::system_clock::now();
}

ChessGame::~ChessGame()
{
}

void ChessGame::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	HDC hdc = GetDC(hWnd);
	m_hMemDC = CreateCompatibleDC(hdc);
	m_hBitmap = CreateCompatibleBitmap(hdc, 1024, 768);
	m_hOld = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	ResourceManager::GetInstance()->Init(m_hMemDC);
	SceneManager::GetInstance()->Init();
	ReleaseDC(hWnd, hdc);
}

void ChessGame::Update()
{
	std::chrono::duration<float> sec = std::chrono::system_clock::now() - m_LastTime;
	m_fElapseTime = sec.count();
	m_LastTime = std::chrono::system_clock::now();

	SceneManager::GetInstance()->Update(m_fElapseTime);
}

void ChessGame::Render()
{
	HDC hdc = GetDC(m_hWnd);
	SceneManager::GetInstance()->Render();
	BitBlt(hdc, 0, 0, 1024, 768, m_hMemDC, 0, 0, SRCCOPY);
	ReleaseDC(m_hWnd, hdc);
}

void ChessGame::Release()
{
	SceneManager::GetInstance()->Release();
	ResourceManager::GetInstance()->Release();
	SelectObject(m_hMemDC, m_hOld);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}