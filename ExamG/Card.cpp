#include "Card.h"
Card::Card()
	:font(nullptr),
	name("Geralt"),
	power(15)
{
	shape.setSize({ 90.f,130.f });
	shape.setFillColor(sf::Color(210, 210, 180));
	shape.setOutlineThickness(2.f);
	shape.setOutlineColor(sf::Color::Black);
}
Card::Card(const sf::Font& fontRef)
	:font(&fontRef),
	name("Geralt"),
	power(15)
{
	shape.setSize({ 90.f,130.f });
	shape.setFillColor(sf::Color(210, 210, 180));
	shape.setOutlineThickness(2.f);
	shape.setOutlineColor(sf::Color::Black);
}
void Card::SetPosition(float x, float y)
{
	shape.setPosition({ x,y });
}
void Card::Draw(sf::RenderWindow& window) const
{
	window.draw(shape);
	if (font)
	{
		sf::Text nameText(*font);
		nameText.setString(name);
		nameText.setPosition({ shape.getPosition().x + 5.f, shape.getPosition().y + 5.f });
		window.draw(nameText);

		sf::Text powerText(*font);
		powerText.setString(std::to_string(power));
		powerText.setPosition({ shape.getPosition().x + 5.f, shape.getPosition().y + 90.f });
		window.draw(powerText);
	}
}
int Card::GetPower() const
{
	return power;
}