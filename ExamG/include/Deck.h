#pragma once
#include <vector>
#include <memory>
#include "Card.h"
class Deck
{
	std::vector<std::unique_ptr<Card>> cards;
	int currentSize;
public:
	Deck();
	Deck(const sf::Font& font);
	void Shuffle();
	std::unique_ptr<Card> GetCard();
	bool IsEmpty()const;
	int GetSize()const;
	void AddCard();

};

