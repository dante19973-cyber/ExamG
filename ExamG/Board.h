#pragma once
#include "Player.h"
#include "Bot.h"
class Board
{
	Player player;
	Bot bot;
public:
	Board();
	void Update();
	void Draw()const;
};

