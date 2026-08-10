#include "../include/Hero.h"
#include "../include/ResourceManager.h"
#include "../include/Board.h"

Hero::Hero(const sf::Font& font, const std::string& name, int power)
	: Card(font, name, power)
{
}

Hero::Hero(const std::string& name, int power,
	const sf::Font& font, ResourceManager& res,
	const std::string& artKey)
	: Card(font, name, power)
{
	nameText.setCharacterSize(14);
	nameText.setFillColor(sf::Color::White);
	powerText.setCharacterSize(22);
	powerText.setFillColor(sf::Color::White);
	SetVis(res, artKey, CardType::Hero, SpecialType::None);
}

// Герой не реагує на ефекти поля — виходить на нього без додаткової дії.
void Hero::Play(Board&, BoardRow)
{
}

// Сила героя незмінна: ні погода, ні ріг на неї не впливають.
int Hero::GetCurPow(const Board&, BoardRow) const
{
	return power;
}

CardType Hero::GetType() const
{
	return CardType::Hero;
}
