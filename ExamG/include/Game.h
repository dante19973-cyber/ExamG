#pragma once
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Board.h"
#include "ResourceManager.h"

// Керує загальним станом застосунку: заставка -> меню -> гра.
// Делегує малювання/оновлення відповідному об'єкту (Menu або Board)
// залежно від поточного стану.
class Game
{
	ResourceManager& resources;

	enum class GameState
	{
		Splash,  // заставка при запуску
		Menu,    // головне меню
		Playing  // власне партія в грі
	};
	GameState currentState;

	Board board;
	Menu menu;

	sf::Clock splashClock;
	std::unique_ptr<sf::Sprite> splashSprite;
	bool splashReady = false;
	bool requestExit = false; // true, коли гравець натиснув "Exit" у меню

public:
	Game(ResourceManager& resources);

	void Update(sf::Time deltaTime);
	void Draw(sf::RenderWindow& window);
	bool HandleClick(sf::Vector2f mousePos);
	void BotMove();

	bool ShouldExit() const { return requestExit; }
};
