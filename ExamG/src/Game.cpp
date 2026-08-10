#include "../include/Game.h"

// Готує заставку та меню одразу при старті, щоб перший кадр уже був готовий.
Game::Game(ResourceManager& resources)
	: resources(resources),
	currentState(GameState::Splash),
	board(resources)
{
	auto mode = sf::VideoMode::getDesktopMode();
	float screenW = static_cast<float>(mode.size.x);
	float screenH = static_cast<float>(mode.size.y);

	this->splashSprite = std::make_unique<sf::Sprite>(resources.GetTexture("splash"));
	ResourceManager::FitSprite(*this->splashSprite, screenW, screenH);
	this->splashReady = true;
	this->splashClock.restart();

	this->menu.Init(resources, screenW, screenH);
}

// Оновлює лише той підоб'єкт, який відповідає поточному стану гри.
void Game::Update(sf::Time deltaTime)
{
	switch (this->currentState)
	{
	case GameState::Splash:
		// Заставка показується рівно 5 секунд, потім автоматично йде меню.
		if (this->splashClock.getElapsedTime().asSeconds() >= 5.f)
			this->currentState = GameState::Menu;
		break;
	case GameState::Menu:
		this->menu.Update();
		break;
	case GameState::Playing:
		this->board.Update();
		break;
	}
}

void Game::Draw(sf::RenderWindow& window)
{
	switch (this->currentState)
	{
	case GameState::Splash:
		if (this->splashReady && this->splashSprite)
			window.draw(*this->splashSprite);
		break;
	case GameState::Menu:
		this->menu.Draw(window);
		break;
	case GameState::Playing:
		this->board.Draw(window);
		break;
	}
}

// Обробляє клік мишею відповідно до поточного стану:
// заставку — пропускає, меню — перевіряє кнопки, гру — делегує Board.
bool Game::HandleClick(sf::Vector2f mousePos)
{
	if (this->currentState == GameState::Splash)
	{
		this->currentState = GameState::Menu;
		return true;
	}

	if (this->currentState == GameState::Menu)
	{
		int result = this->menu.HandleClick(mousePos);
		if (result == 1) // "Play"
		{
			this->board.ResetMatch();
			this->resources.PlayMusic();
			this->currentState = GameState::Playing;
			return true;
		}
		if (result == 2) // "Exit"
		{
			this->requestExit = true;
			return true;
		}
	}
	else if (this->currentState == GameState::Playing)
	{
		if (this->board.IsGameOver())
		{
			if (this->board.HandleEndClick(mousePos))
			{
				this->resources.StopMusic();
				this->currentState = GameState::Menu;
				return true;
			}
			return true;
		}

		bool handled = this->board.HandleClick(mousePos);
		if (this->board.WantMenu())
		{
			this->board.ClearMenuRequest();
			this->resources.StopMusic();
			this->currentState = GameState::Menu;
			return true;
		}
		return handled;
	}
	return false;
}

void Game::BotMove()
{
	if (this->currentState == GameState::Playing)
		this->board.BotMove();
}
