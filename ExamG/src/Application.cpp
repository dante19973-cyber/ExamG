#include "../include/Application.h"

// Створює повноекранне вікно, завантажує всі ресурси й готує гру до старту.
Application::Application()
	: window(sf::VideoMode::getDesktopMode(),
		WINDOW_TITLE,
		sf::State::Fullscreen)
{
	srand(static_cast<unsigned>(time(nullptr)));
	this->resources.LoadResources();
	this->game = std::make_unique<Game>(this->resources);
}

// Головний ігровий цикл: доки вікно відкрите — обробляємо події,
// оновлюємо стан і малюємо кадр.
void Application::Run()
{
	while (this->window.isOpen())
	{
		this->ProcessEvents();
		this->Update(sf::Time::Zero);
		this->Render();
	}
}

// Обробляє системні події вікна (закриття, клік мишею) та передає їх у Game.
void Application::ProcessEvents()
{
	if (this->game->ShouldExit())
		this->window.close();

	while (const auto event = this->window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			this->window.close();

		if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(this->window);
			sf::Vector2f worldPos = this->window.mapPixelToCoords(mousePos);
			if (mouseEvent->button == sf::Mouse::Button::Left)
			{
				this->game->HandleClick(worldPos);
			}
		}
	}
}

void Application::Update(sf::Time deltaTime)
{
	this->game->Update(deltaTime);
	if (this->game->ShouldExit())
		this->window.close();
}

void Application::Render()
{
	this->window.clear();
	this->game->Draw(this->window);
	this->window.display();
}
