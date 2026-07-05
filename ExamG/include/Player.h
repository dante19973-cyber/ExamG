#pragma once
#include "Deck.h"
#include <string>
#include "BoardRow.h"
class Board;
class Player
{
	std::string name;
	Deck deck;
	bool passed = false;
	int lives = 2;
	std::vector<std::unique_ptr<Card>> hand;
	std::vector<std::unique_ptr<Card>> meleeRow;
	std::vector<std::unique_ptr<Card>> rangedRow;
public:
	Player(const sf::Font& font);
	void TestHand();
	void AddCard(std::unique_ptr<Card> card);
	bool PlayCard(BoardRow row, Board& board);
	void DrawCard();
	void Draw(sf::RenderWindow& window);
	bool HandleClick(sf::Vector2f mousePos);
	int MeleeP(const Board& board) const;
	int RangedP(const Board& board) const;
	int TotalP(const Board& board) const;
	void Pass();
	bool HasPassed()const;
	void LiveLose();
	int GetLives()const;
	int HandSize()const;
	void CleanBoard();
};

