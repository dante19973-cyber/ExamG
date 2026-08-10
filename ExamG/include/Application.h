#pragma once
#include "Game.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>

// Точка входу гри: створює вікно, ресурси й головний об'єкт Game,
// та керує ігровим циклом (події -> оновлення -> малювання).
class Application
{
	static constexpr const char* WINDOW_TITLE = "Gwent Clone";

	sf::RenderWindow window;
	ResourceManager resources;
	std::unique_ptr<Game> game;

	void ProcessEvents();
	void Update(sf::Time deltaTime);
	void Render();

public:
	Application();
	void Run();
};
