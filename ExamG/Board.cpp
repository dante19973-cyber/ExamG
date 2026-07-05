#include "Board.h"
#include "ResourceManager.h"
Board::Board(ResourceManager& resources)
	:resources(resources),
	testCard(resources.GetFont("main"))
{
	background.setSize({ 1200.f,650.f });
	background.setPosition({ 40.f,35.f });
	background.setFillColor(sf::Color(35, 90, 35));

	enemyMelee.setSize({ 1000.f,80.f });
	enemyMelee.setPosition({ 140.f,120.f });
	enemyMelee.setFillColor(sf::Color(80, 120, 80));

	enemyRanged.setSize({ 1000.f,80.f });
	enemyRanged.setPosition({ 140.f,220.f });
	enemyRanged.setFillColor(sf::Color(70, 110, 70));

	playerRanged.setSize({ 1000.f,80.f });
	playerRanged.setPosition({ 140.f,380.f });
	playerRanged.setFillColor(sf::Color(70, 110, 70));

	playerMele.setSize({ 1000.f,80.f });
	playerMele.setPosition({ 140.f,480.f });
	playerMele.setFillColor(sf::Color(80, 210, 80));
	testCard.SetPosition(560.f, 500.f);
}
void Board::Update()
{
}
void Board::Draw(sf::RenderWindow& window)
{
	window.draw(background);
	window.draw(enemyMelee);
	window.draw(enemyRanged);
	window.draw(playerRanged);
	window.draw(playerMele);
	testCard.Draw(window);
}
