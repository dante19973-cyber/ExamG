#include "Game.h"
Game::Game(ResourceManager& resources)
	:resources(resources),
	currentState(GameState::Menu),
	board(resources)
{

}
void Game::Update(sf::Time deltaTime)
{
	switch (currentState)
	{
	case GameState::Menu:
		menu.Update();
		break;
	case GameState::Playing:
		board.Update();
		break;
	}
}
void Game::Draw(sf::RenderWindow& window)
{
	switch (currentState)
	{
	case GameState::Menu:
		menu.Draw(window);
		break;

	case GameState::Playing:
		board.Draw(window);
		break;
	}
}
bool Game::HandleClick(sf::Vector2f mousePos)
{
	if (currentState == GameState::Menu)
	{
		if (menu.HandleClick(mousePos))
		{
			currentState = GameState::Playing;
			return true;
		}
	}
	return false;
}