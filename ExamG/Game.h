#pragma once
#include "Menu.h"
#include "Board.h"
#include "Statistics.h"
class Game
{
	Board board;
	Menu menu;
	Statistics statistics;
public:
	Game();
	void Update();
	void Draw()const;
};

