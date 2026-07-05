#include "Application.h"
#include <iostream>
Application::Application()
	: window(sf::VideoMode::getDesktopMode(),
		WINDOW_TITLE,
		sf::State::Fullscreen)
{
	resources.LoadResources();
	game = std::make_unique<Game>(resources);
}
void Application::Run()
{
	while (window.isOpen())
	{
		ProcessEvents();
		Update(sf::Time::Zero);
		Render();
	}
}
void Application::ProcessEvents()
{
	while (const auto event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			window.close();

		if (event->is<sf::Event::MouseButtonPressed>())
		{
			auto mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

			game->HandleClick(worldPos);
		}
	}
}
void Application::Update(sf::Time deltaTime)
{
	game->Update(deltaTime);
}
void Application::Render()
{
	window.clear();
	game->Draw(window);
	window.display();
}
