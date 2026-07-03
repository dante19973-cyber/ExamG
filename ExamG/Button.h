#pragma once
#include <SFML/Graphics.hpp>
class Button
{
	sf::RectangleShape shape;
	//sf::Text text;
public:
	Button();
	bool IsPressed()const;
	void Draw()const;
};