#pragma once
#include "Deck.h"
#include <string>
class Player
{
	std::string name;
	Deck deck;
public:
	Player();
	void PlayCard();
	void Draw()const;
};

