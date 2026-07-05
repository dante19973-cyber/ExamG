#include "../include/Board.h"
#include "../include/ResourceManager.h"
#include<iostream>
Board::Board(ResourceManager& resources)
	:resources(resources),
	player(resources.GetFont("main")),
	bot(resources.GetFont("main"))
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
	playerMele.setFillColor(sf::Color::Red);

	passButton.setSize({ 120.f,45.f });
	passButton.setPosition({ 1070.f,600.f });
	passButton.setFillColor(sf::Color(140, 40, 40));
	passButton.setOutlineThickness(2.f);
	passButton.setOutlineColor(sf::Color::White);
}
void Board::Update()
{
	if (player.HasPassed() && !bot.HasPassed()&&botClock.getElapsedTime().asSeconds()>1.f)
	{
		bot.MakeMove(player.TotalP(*this), player.HasPassed());
		botClock.restart();
		CheckEnd();
	}
	std::cout << "Player hand: " << player.HandSize()
		<< "  Bot hand: " << bot.HandSize() << std::endl;
	if (player.HandSize() == 0)
		player.Pass();
	if (bot.HandSize() == 0)
		bot.Pass();
	CheckEnd();

}
void Board::Draw(sf::RenderWindow& window)
{
	window.draw(background);
	window.draw(enemyMelee);
	window.draw(enemyRanged);
	window.draw(playerRanged);
	window.draw(playerMele);
	player.Draw(window);
	bot.Draw(window);
	sf::Text meleeText(resources.GetFont("main"));
	meleeText.setCharacterSize(28);
	meleeText.setFillColor(sf::Color::White);
	meleeText.setPosition({ 1170.f,500.f });
	meleeText.setString(std::to_string(player.MeleeP(*this)));
	window.draw(meleeText);

	sf::Text rangedText(resources.GetFont("main"));
	rangedText.setCharacterSize(28);
	rangedText.setFillColor(sf::Color::White);
	rangedText.setPosition({ 1170.f,400.f });
	rangedText.setString(std::to_string(player.RangedP(*this)));
	window.draw(rangedText);

	sf::Text totalText(resources.GetFont("main"));
	totalText.setCharacterSize(36);
	totalText.setFillColor(sf::Color::Yellow);
	totalText.setPosition({ 1170.f,620.f });
	totalText.setString(std::to_string(player.TotalP(*this)));
	window.draw(totalText);

	sf::Text meleeBotText(resources.GetFont("main"));
	meleeBotText.setCharacterSize(28);
	meleeBotText.setFillColor(sf::Color::White);
	meleeBotText.setPosition({ 1170.f,140.f });
	meleeBotText.setString(std::to_string(bot.MeleeP()));
	window.draw(meleeBotText);

	sf::Text rangedBotText(resources.GetFont("main"));
	rangedBotText.setCharacterSize(28);
	rangedBotText.setFillColor(sf::Color::White);
	rangedBotText.setPosition({ 1170.f,240.f });
	rangedBotText.setString(std::to_string(bot.RangedP()));
	window.draw(rangedBotText);

	sf::Text totalBotText(resources.GetFont("main"));
	totalBotText.setCharacterSize(36);
	totalBotText.setFillColor(sf::Color::Yellow);
	totalBotText.setPosition({ 1170.f,40.f });
	totalBotText.setString(std::to_string(bot.TotalP()));
	window.draw(totalBotText);

	sf::Text passText(resources.GetFont("main"));
	passText.setCharacterSize(24);
	passText.setFillColor(sf::Color::Yellow);
	passText.setString("Pass");
	passText.setPosition({ 1095.f,607.f });
	window.draw(passButton);
	window.draw(passText);
}
bool Board::HandleClick(sf::Vector2f mousePos)
{
	if (player.HasPassed())
		return false;
	if (passButton.getGlobalBounds().contains(mousePos))
	{
		player.Pass();
		passButton.setFillColor(sf::Color(70, 70, 70));
		bot.MakeMove(player.TotalP(*this), player.HasPassed());
		CheckEnd();
		return true;
	}
	if (player.HandleClick(mousePos))
		return true;
	BoardRow row = GetRowAtPosition(mousePos);
	if (row == BoardRow::PlayerMelee)
	{
		if (player.PlayCard(BoardRow::PlayerMelee, *this))
			{
				bot.MakeMove(player.TotalP(*this), player.HasPassed());
				CheckEnd();
			}
		return true;
	}
	if (row == BoardRow::PlayerRanged)
	{
		if (player.PlayCard(BoardRow::PlayerRanged, *this))
			{
				bot.MakeMove(player.TotalP(*this), player.HasPassed());
				CheckEnd();
			}
		return true;
	}
	return false;
}
BoardRow Board::GetRowAtPosition(sf::Vector2f mousePos) const
{
	if (playerMele.getGlobalBounds().contains(mousePos))
		return BoardRow::PlayerMelee;
	if (playerRanged.getGlobalBounds().contains(mousePos))
		return BoardRow::PlayerRanged;
	if (enemyMelee.getGlobalBounds().contains(mousePos))
		return BoardRow::EnemyMelee;
	if (enemyRanged.getGlobalBounds().contains(mousePos))
		return BoardRow::EnemyRanged;
	return BoardRow::None;
}
void Board::BotMove()
{
	bot.MakeMove(player.TotalP(*this), player.HasPassed());
}
void Board::CheckEnd()
{
	if (roundEnd)
		return;
	bool playerDone = player.HasPassed() || player.HandSize() == 0;
	bool botDone = bot.HasPassed() || bot.HandSize() == 0;

	if (!playerDone || !botDone)
		return;
	if (player.TotalP(*this) > bot.TotalP())
	{ 
		std::cout << "Player\n";
		bot.LiveLose();
		std::cout << "Player" << player.GetLives() << std::endl;
		std::cout << "Bot" << bot.GetLives() << std::endl;
	}
	else if (player.TotalP(*this) < bot.TotalP())
	{
		std::cout << "Bot\n";
		player.LiveLose();
		std::cout << "Player" << player.GetLives() << std::endl;
		std::cout << "Bot" << bot.GetLives() << std::endl;
	}
	else
	{
		std::cout << "Draw\n";
		std::cout << "Player" << player.GetLives() << std::endl;
		std::cout << "Bot" << bot.GetLives() << std::endl;
	}
	if (IsOver())
	{
		if (player.GetLives() == 0)
			std::cout << "Win Bot\n";
		else
			std::cout << "Win Player\n";
	}
	roundEnd = true;
	NewRound();
}
void Board::NewRound()
{
	player.CleanBoard();
	bot.CleanBoard();
	if (!player.GetLives() || !bot.GetLives())
		return;
	player.DrawCard();
	bot.DrawCard();
	passButton.setFillColor(sf::Color::Green);
	roundEnd = false;
}
bool Board::IsOver()const
{
	return player.GetLives() == 0 || bot.GetLives() == 0;
}
void Board::EnableWeather()
{
	weatherActive = true;
}
bool Board::IsWeatherAct() const
{
	return weatherActive;
}
void Board::IsWeatherDis()
{
	weatherActive = false;
}

void Board::EnableHorn()
{
	hornActive = true;
}
void Board::EnableDecoy()
{
	decoyActive = true;
}
void Board::EnableScorch()
{
	scorchActive = true;
}
