#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "CardType.h"
class Board;
class Card
{
	std::string name;
	int power;
	const sf::Font* font;
	sf::RectangleShape shape;
	sf::Text nameText;
	sf::Text powerText;
	sf::Vector2f position;
	bool selected;
public:
	Card(const sf::Font& font, const std::string& cardName="Unknown", int cardPower=0);
	void SetPosition(float x, float y);
	void Draw(sf::RenderWindow& window) const;
	int GetPower() const;
	void SetSelected(bool value);
	bool IsSelected()const;
	bool Contains(sf::Vector2f mousePos) const;
	std::string GetName() const;
	virtual ~Card() = default;
	virtual void Play(Board& board) = 0;
	virtual CardType GetType() const=0;
	virtual int GetCurPow(const Board& doard) const;
};

