#include "../include/Button.h"
Button::Button()
{
	shape.setSize({ 300.f,80.f });
	shape.setFillColor(sf::Color(60, 60, 60));
	shape.setPosition({ 490.f,320.f });
}
bool Button::IsPressed(sf::Vector2f mousePos)
{
	return shape.getGlobalBounds().contains(mousePos);
}
void Button::Draw(sf::RenderWindow& window)
{
	window.draw(shape);
}