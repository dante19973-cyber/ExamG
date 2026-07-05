#pragma once
#include <string>
#include <SFML/Graphics.hpp>
class Card
{
	std::string name;
	int power;
	const sf::Font* font;
	sf::RectangleShape shape;
public:
	Card();
	Card(const sf::Font& font);
	void SetPosition(float x, float y);
	void Draw(sf::RenderWindow& window) const;
	int GetPower() const;
};
