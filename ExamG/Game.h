#pragma once
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Board.h"
#include "Statistics.h"
class ResourceManager;
class Game
{
	ResourceManager& resources;
	enum class GameState
	{
		Menu,
		Playing
	};
	GameState currentState;
	Board board;
	Menu menu;
	Statistics statistics;
public:
	Game(ResourceManager& resources);
	void Update(sf::Time deltaTime);
	void Draw(sf::RenderWindow& window);
	bool HandleClick(sf::Vector2f mousePos);
};

