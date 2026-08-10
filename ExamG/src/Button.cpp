#include "../include/Button.h"

// Створює кнопку: фон-прямокутник + текст, вирівняний по центру.
Button::Button(const sf::Font& font, const std::string& text,
	sf::Vector2f size, sf::Vector2f pos)
	: label(std::in_place, font, text, 100)
{
	this->shape.setSize(size);
	this->shape.setPosition(pos);
	this->shape.setFillColor(sf::Color(50, 100, 50));
	this->shape.setOutlineThickness(3.f);
	this->shape.setOutlineColor(sf::Color::White);

	this->hasText = true;
	this->label->setFillColor(sf::Color::Yellow);

	// Центруємо текст відносно його власних меж, а тоді ставимо
	// в центр прямокутника кнопки.
	sf::FloatRect bounds = this->label->getLocalBounds();
	this->label->setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
	this->label->setPosition({
		pos.x + size.x / 2.f,
		pos.y + size.y / 2.f
		});
}

// Переміщує кнопку в нову позицію (фон і текст разом).
void Button::SetPos(sf::Vector2f pos)
{
	this->shape.setPosition(pos);
	if (this->hasText && this->label)
	{
		sf::Vector2f size = this->shape.getSize();
		this->label->setPosition({
			pos.x + size.x / 2.f,
			pos.y + size.y / 2.f
			});
	}
}

// Перевіряє, чи натиснута кнопка (клік потрапив у її межі).
bool Button::IsPressed(sf::Vector2f mousePos)
{
	return this->shape.getGlobalBounds().contains(mousePos);
}

void Button::Draw(sf::RenderWindow& window)
{
	window.draw(this->shape);
	if (this->hasText && this->label)
		window.draw(*this->label);
}
