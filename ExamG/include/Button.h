#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <optional>

// Проста кнопка з прямокутним фоном і текстовою підписом.
// std::optional<sf::Text> потрібен тому, що sf::Text не має конструктора
// без параметрів, а нам треба вміти створювати "порожню" кнопку за замовчуванням.
class Button
{
	sf::RectangleShape shape;
	std::optional<sf::Text> label;
	bool hasText = false;
public:
	Button() = default;
	Button(const sf::Font& font, const std::string& text, sf::Vector2f size, sf::Vector2f pos);

	void SetPos(sf::Vector2f pos);
	bool IsPressed(sf::Vector2f mousePos);
	void Draw(sf::RenderWindow& window);
};
