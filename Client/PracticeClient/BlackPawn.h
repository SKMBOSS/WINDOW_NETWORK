#pragma once
#include "ChessPiece.h"

class BlackPawn :
	public ChessPiece
{
public:
	virtual void init(Tile* tile);
	virtual void Update();
	virtual void Render();
	virtual void Move();
public:
	BlackPawn();
	~BlackPawn();
};

