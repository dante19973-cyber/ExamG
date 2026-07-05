#pragma once
#include <SFML/Graphics.hpp>
#include "Card.h"
#include "Player.h"
#include "Bot.h"
class ResourceManager;
class Board
{
	Player player;
	Bot bot;
	Card testCard;
	ResourceManager& resources;
	sf::RectangleShape background;
	sf::RectangleShape playerMele;
	sf::RectangleShape playerRanged;
	sf::RectangleShape enemyMelee;
	sf::RectangleShape enemyRanged;
	
public:
	Board(ResourceManager& resources);
	void Update();
	void Draw(sf::RenderWindow& window);

};

