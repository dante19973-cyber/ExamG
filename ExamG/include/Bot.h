#pragma once
#include "Deck.h"
#include <string>
#include "BoardRow.h"

class Board;
class ResourceManager;

// Простий комп'ютерний суперник. Структурно майже дзеркалить Player
// (рука, три ряди, колода, скидання), але замість HandleClick має
// MakeMove() — метод, що сам приймає рішення, яку карту зіграти.
class Bot
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
	Bot(ResourceManager& res);

	// Знищує всіх юнітів заданої сили в рядах бота (ефект спалення).
	std::vector<sf::FloatRect> CollectAndRemoveByPower(int power, const Board& board);

	// Приймає одне ігрове рішення: розіграти найкращу карту з руки
	// або спасувати, якщо гравець уже спасував і виграє за очками.
	void MakeMove(int playerPow, Board& board, bool playerPass = false);

	void TestHand();
	void DrawCard();
	void Draw(sf::RenderWindow& window);

	int MeleeP(const Board& board) const;
	int RangedP(const Board& board) const;
	int SiegeP(const Board& board) const;
	int TotalP(const Board& board) const;

	void Pass();
	bool HasPassed() const;
	void LiveLose();
	int GetLives() const;
	int HandSize() const;
	void CleanBoard();
	int MaxUnitPower(const Board& board) const;
	void FullReset(ResourceManager& res);

	int DeckSize() const;
	int DiscardSize() const { return static_cast<int>(discard.size()); }
	void AddToDiscard(std::unique_ptr<Card> c);
	const Card* TopDiscard() const; // верхня карта скидання (для малюнка)
};
