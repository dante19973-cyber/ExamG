#pragma once
#include "Card.h"
class UnitCard: public Card
{
public:
	UnitCard(const sf::Font& font, const std::string& name, int power);
	void Play(Board& board) override;
	CardType GetType() const override;
	int GetCurPow(const Board& board) const override;
};

