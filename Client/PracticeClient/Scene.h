#pragma once
#include <Windows.h>

class Scene
{
public:
	virtual void Init() = 0;
	virtual void Update(float fElapseTime) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
public:
	Scene();
	~Scene();
};
