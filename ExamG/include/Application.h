#pragma once
#include "Game.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>
class Application
{
	static constexpr unsigned WINDOW_WIDTH = 1280;
	static constexpr unsigned WINDOW_HEIGHT = 720;
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