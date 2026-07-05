#pragma once
#include "Deck.h"
#include<string>
class Bot
{	
	std::string name;
	Deck deck;
	bool passed = false;
	int lives = 2;
	std::vector<std::unique_ptr<Card>> hand;
	std::vector<std::unique_ptr<Card>> meleeRow;
	std::vector<std::unique_ptr<Card>> rangedRow;
public:
	Bot(const sf::Font& font);
	void MakeMove(int playerPow,bool playerPass=false);
	void TestHand();
	void DrawCard();
	void Draw(sf::RenderWindow& window) ;
	int MeleeP() const;
	int RangedP() const;
	int TotalP() const;
	void Pass();
	bool HasPassed()const;
	void LiveLose();
	int GetLives()const;
	int HandSize()const;
	void CleanBoard();
};