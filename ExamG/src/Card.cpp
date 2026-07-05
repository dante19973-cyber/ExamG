#include "../include/Card.h"

Card::Card(const sf::Font& fontRef, const std::string& cardName, int cardPower)
	:font(&fontRef),
	name(cardName),
	power(cardPower),
	nameText(fontRef, sf::String(), 14),
	powerText(fontRef, sf::String(), 24),
	position({0.f,0.f}),
	selected(false)
{
	shape.setSize({ 90.f,130.f });
	shape.setFillColor(sf::Color(210, 210, 180));
	shape.setOutlineThickness(2.f);
	shape.setOutlineColor(sf::Color::Black);

	nameText.setCharacterSize(14);
	nameText.setFillColor(sf::Color::Black);
	nameText.setString(name);

	powerText.setString(std::to_string(power));
	powerText.setCharacterSize(24);
	powerText.setFillColor(sf::Color::Red);
}

void Card::SetPosition(float x, float y)
{
	position = { x,y };
	shape.setPosition(position);
	nameText.setPosition({x + 5.f,y + 5.f});
	powerText.setPosition({x + 35.f,y + 90.f});
}
void Card::Draw(sf::RenderWindow& window) const
{
	window.draw(shape);
	window.draw(nameText);
	window.draw(powerText);
}
int Card::GetPower() const
{
	return power;
}
void Card::SetSelected(bool value)
{
	selected = value;
	if (selected)
	{
		shape.setOutlineColor(sf::Color::Yellow);
		shape.setPosition({ position.x, position.y - 20.f });
		nameText.setPosition({ position.x + 5.f, position.y - 20.f + 5.f });
		powerText.setPosition({ position.x + 35.f, position.y - 20.f + 90.f });
	}
	else
	{
		shape.setOutlineColor(sf::Color::Black);
		shape.setPosition(position);
		nameText.setPosition({ position.x + 5.f, position.y + 5.f });
		powerText.setPosition({ position.x + 35.f, position.y + 90.f });
	}
}
bool Card::IsSelected() const
{
	return selected;
}
bool Card::Contains(sf::Vector2f mousePos) const
{
	return shape.getGlobalBounds().contains(mousePos);
}
std::string Card::GetName() const
{
	return name;
}
int Card::GetCurPow(const Board& board) const
{
	return power;
}