#pragma once
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include "CardType.h"
#include "BoardRow.h"

class Board;
class ResourceManager;

// Абстрактний базовий клас усіх ігрових карток.
// Зберігає спільні для всіх карт дані (назва, сила, зовнішній вигляд)
// та оголошує чисто віртуальні методи Play()/GetType(), які змушують
// кожен конкретний тип карти (UnitCard, Hero, SpecialCard) визначити
// власну поведінку — це і є абстракція та поліморфізм у дії.
class Card
{
protected:
	std::string name;
	int power;
	const sf::Font* font;

	// Візуальні елементи карти.
	sf::RectangleShape shape;
	sf::Text nameText;
	sf::Text powerText;
	sf::Vector2f position;
	bool selected;

	std::unique_ptr<sf::Sprite> art;
	std::unique_ptr<sf::Sprite> powerB;
	std::unique_ptr<sf::Sprite> typeI;
	bool hasArt = false;
	bool hasBage = false;
	bool hasIcon = false;

	static constexpr float CardW = 150.f;
	static constexpr float CardH = 225.f;

public:
	Card(const sf::Font& font, const std::string& cardName = "Unknown", int cardPower = 0);
	virtual ~Card() = default;

	void SetPosition(float x, float y);
	void Draw(sf::RenderWindow& window) const;

	// Аксесори.
	int GetPower() const;
	std::string GetName() const;
	bool IsSelected() const;
	sf::FloatRect GetBounds() const { return shape.getGlobalBounds(); }

	void SetSelected(bool value);
	bool Contains(sf::Vector2f mousePos) const;

	// Чисто віртуальні методи — робота карти на полі та її тип
	// визначаються повністю у нащадках (абстракція).
	virtual void Play(Board& board, BoardRow row) = 0;
	virtual CardType GetType() const = 0;

	// Сила карти з урахуванням ефектів поля; базова реалізація повертає
	// "суху" силу, нащадки можуть додати свою логіку (погода, ріг тощо).
	virtual int GetCurPow(const Board& board, BoardRow row) const;

	// За замовчуванням карта не має спецефекту; SpecialCard це перевизначає.
	virtual SpecialType GetSpecialType() const { return SpecialType::None; }

	// Готує/оновлює зовнішній вигляд карти (арт, значок сили, іконку типу).
	void SetVis(ResourceManager& res, const std::string& artKey, CardType type, SpecialType st = SpecialType::None);
	void LayVis();
};
