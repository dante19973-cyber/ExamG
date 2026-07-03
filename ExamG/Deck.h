#pragma once
#include <vector>
#include "Card.h"
class Deck
{
	std::vector<Card> cards;
	int currentSize;
public:
	Deck();
	void AddCard();
	Card GetCard();
};

