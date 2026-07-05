#pragma once
#include <SFML/Graphics.hpp>
class Button
{
	sf::RectangleShape shape;
	//sf::Text text;
public:
	Button();
	bool IsPressed(sf::Vector2f mousePos);
	void Draw(sf::RenderWindow& window);
};