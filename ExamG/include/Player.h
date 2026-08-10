#pragma once
#include "Deck.h"
#include <string>
#include <vector>
#include <memory>
#include "BoardRow.h"

class Board;
class ResourceManager;

// Гравець-людина: тримає руку карт, три ряди на полі (ближній/дальній/
// облога), колоду й скидання. Відповідає за всю логіку розігрування
// карт, спричинену кліками миші.
class Player
{
	std::string name;
	Deck deck;
	bool passed = false;
	int lives = 2;

	std::vector<std::unique_ptr<Card>> hand;
	std::vector<std::unique_ptr<Card>> meleeRow;
	std::vector<std::unique_ptr<Card>> rangedRow;
	std::vector<std::unique_ptr<Card>> siegeRow;
	std::vector<std::unique_ptr<Card>> discard;

public:
	float handY = 0.f;

	Player(ResourceManager& res);

	// Розкладає карти руки в один горизонтальний ряд знизу екрана.
	void TestHand();
	void AddCard(std::unique_ptr<Card> card);

	// Розігрує виділену карту руки в заданий ряд. Повертає false,
	// якщо гравець спасував або жодна карта не була вибрана/підходить.
	bool PlayCard(BoardRow row, Board& board);
	void DrawCard();
	void Draw(sf::RenderWindow& window);

	// Обробляє клік по руці: виділяє карту, під якою клікнули.
	bool HandleClick(sf::Vector2f mousePos);

	// Сума сили карт по кожному ряду (з урахуванням ефектів поля)
	// та загальна сума за всі три ряди.
	int MeleeP(const Board& board) const;
	int RangedP(const Board& board) const;
	int SiegeP(const Board& board) const;
	int TotalP(const Board& board) const;

	void Pass();
	bool HasPassed() const;
	void LiveLose();
	int GetLives() const;
	int HandSize() const;

	// Переносить усі карти з поля у скидання й готує гравця до нового раунду.
	void CleanBoard();

	// Найбільша сила серед юнітів (без героїв) на всіх рядах —
	// потрібно для карти "Спалення".
	int MaxUnitPower(const Board& board) const;

	// Розігрує "Підміну": міняє місцями карту на полі під курсором
	// із виділеною картою-підміною в руці.
	bool PlayDecoy(sf::Vector2f mousePos, Board& board);

	// Повністю скидає гравця до стану нової партії (нова колода, рука, життя).
	void FullReset(ResourceManager& res);

	// Розігрує виділену погодну карту (без прив'язки до конкретного ряду).
	bool TryPlayWeather(Board& board);

	// Знищує всіх юнітів заданої сили в трьох рядах гравця (ефект спалення)
	// і повертає прямокутники знищених карт (для візуального ефекту).
	std::vector<sf::FloatRect> CollectAndRemoveByPower(int power, const Board& board);

	int DeckSize() const;
	int DiscardSize() const { return static_cast<int>(discard.size()); }
	void AddToDiscard(std::unique_ptr<Card> c);
	const Card* TopDiscard() const; // верхня карта скидання (для малюнка)
};
