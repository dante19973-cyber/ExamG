#pragma once
#include<string>
#include"Card.h"
#include "CardType.h"
class Board;
enum class SpecialType
{
	Weather,
	Decoy,
	Horn,
	Scorch
};
class SpecialCard : public Card
{
	std::string effect;
	int bonus;
	SpecialType type;
public:
	SpecialCard(const sf::Font& font, const std::string& name,SpecialType type);
	void Activate();
	void Play(Board& board)override;
	CardType GetType() const override;
	int GetCurPow(const Board&) const override;
};

