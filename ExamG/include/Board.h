#pragma once
#include <SFML/Graphics.hpp>
#include "Card.h"
#include "Player.h"
#include "Bot.h"
#include "BoardRow.h"
class ResourceManager;
class Board
{
	Player player;
	Bot bot;
	bool roundEnd = false;
	ResourceManager& resources;
	sf::RectangleShape background;
	sf::RectangleShape playerMele;
	sf::RectangleShape playerRanged;
	sf::RectangleShape enemyMelee;
	sf::RectangleShape enemyRanged;
	sf::RectangleShape passButton;
	sf::Clock botClock;
	bool weatherActive = false;
	bool hornActive = false;
	bool scorchActive = false;
	bool decoyActive = false;
public:
	Board(ResourceManager& resources);
	void Update();
	void Draw(sf::RenderWindow& window);
	void BotMove();
	bool HandleClick(sf::Vector2f mousePos);
	BoardRow GetRowAtPosition(sf::Vector2f mousePos) const;
	void CheckEnd();
	void NewRound();
	bool IsOver() const;
	void EnableWeather();
	bool IsWeatherAct() const;
	void IsWeatherDis();
	void EnableHorn();
	void EnableDecoy();
	void EnableScorch();
	bool IsWeatherAct() const;
};

