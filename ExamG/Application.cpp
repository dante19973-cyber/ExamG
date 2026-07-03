#include "Application.h"
Application::Application()
	: window(sf::VideoMode::getDesktopMode(),
		WINDOW_TITLE,
		sf::State::Fullscreen)
{
	resources.LoadResources();
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
	while (const auto event=window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			window.close();
	}
}
void Application::Update(sf::Time deltaTime)
{
	
}
void Application::Render()
{
	window.clear();
	window.display();
}