#pragma once
#include "Deck.h"
#include<string>
class Bot
{
	std::string name;
	Deck deck;
public:
	Bot();
	void MakeMove();
	void Draw() const;
};

