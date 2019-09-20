#pragma once
#include "Scene.h"

using namespace std;

class MainGame :
	public Scene
{
public:
	virtual void Init();
	virtual void Update(float fElapseTime);
	virtual void Render();
	virtual void Release();

public:
	MainGame();
	~MainGame();
};

