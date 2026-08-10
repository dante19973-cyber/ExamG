#include "../include/Board.h"
#include "../include/ResourceManager.h"
#include<algorithm>

Board::Board(ResourceManager& resources)
	:resources(resources),
	player(resources),
	bot(resources)

{
	auto mode = sf::VideoMode::getDesktopMode();
	screenWidth = static_cast<float>(mode.size.x);
	screenHeight = static_cast<float>(mode.size.y);
	panelWidth = screenWidth * 0.16f;
	fieldX  = screenWidth * 0.37f;
	fieldWidth = screenWidth * 0.419f;
	rowHeight = screenHeight * 0.108f;
	rowGap = screenHeight* 0.017f;
	effectWidth = screenWidth * 0.065f;
	enemyStartY = screenHeight * 0.012f;
	playerStartY = enemyStartY + 3.f * (rowHeight + rowGap) + screenHeight * 0.015f;
	handY = screenHeight * 0.790f;
	scoreX = fieldX + fieldWidth + screenWidth * 0.35f;
	const float effectX = fieldX - effectWidth - screenWidth * 0.008f;
	player.handY = handY;
	player.TestHand();

	boardBackground = std::make_unique<sf::Sprite>(resources.GetTexture("board"));

	sf::Vector2u texSize = boardBackground->getTexture().getSize();
	boardBackground->setScale({
		screenWidth / static_cast<float>(texSize.x),
		screenHeight / static_cast<float>(texSize.y)
		});
	boardBackground->setPosition({ 0.f, 0.f });

	leftPanel.setPosition({ 0.f,0.f });
	leftPanel.setSize({ panelWidth, screenHeight });
	leftPanel.setFillColor(sf::Color(0,0,0,0));

	
	enemySiege.setSize({ fieldWidth, rowHeight });
	enemySiege.setPosition({ fieldX, enemyStartY });
	enemySiege.setFillColor(sf::Color(0,0,0,0));

	enemyEfSiege.setSize({ effectWidth, rowHeight });
	enemyEfSiege.setPosition({ effectX, enemyStartY });
	enemyEfSiege.setFillColor(sf::Color(0, 0, 0, 0));

	enemyRanged.setSize({ fieldWidth, rowHeight });
	enemyRanged.setPosition({ fieldX, enemyStartY + rowHeight + rowGap });
	enemyRanged.setFillColor(sf::Color(0, 0, 0, 0));

	enemyEfRanged.setSize({ effectWidth, rowHeight });
	enemyEfRanged.setPosition({ effectX, enemyStartY + rowHeight + rowGap });
	enemyEfRanged.setFillColor(sf::Color(0, 0, 0, 0));

	enemyMelee.setSize({ fieldWidth, rowHeight });
	enemyMelee.setPosition({ fieldX, enemyStartY + 2.f * (rowHeight + rowGap) });
	enemyMelee.setFillColor(sf::Color(0, 0, 0, 0));

	enemyEfMelee.setSize({ effectWidth, rowHeight });
	enemyEfMelee.setPosition({ effectX, enemyStartY + 2.f * (rowHeight + rowGap) });
	enemyEfMelee.setFillColor(sf::Color(0, 0, 0, 0));

	playerMele.setSize({ fieldWidth,rowHeight });
	playerMele.setPosition({ fieldX,playerStartY });
	playerMele.setFillColor(sf::Color(0, 0, 0, 0));

	playerEfMele.setSize({ effectWidth,rowHeight });
	playerEfMele.setPosition({ effectX,	playerStartY });
	playerEfMele.setFillColor(sf::Color(0, 0, 0, 0));

	playerRanged.setSize({ fieldWidth,rowHeight });
	playerRanged.setPosition({ fieldX,playerStartY + rowHeight + rowGap });
	playerRanged.setFillColor(sf::Color(0, 0, 0, 0));

	playerEfRanged.setSize({ effectWidth,rowHeight });
	playerEfRanged.setPosition({ effectX,playerStartY + rowHeight + rowGap });
	playerEfRanged.setFillColor(sf::Color(0, 0, 0, 0));

	playerSiege.setSize({ fieldWidth,rowHeight });
	playerSiege.setPosition({ fieldX,playerStartY + (rowHeight + rowGap) * 2 });
	playerSiege.setFillColor(sf::Color(0, 0, 0, 0));

	playerEfSiege.setSize({ effectWidth,rowHeight });
	playerEfSiege.setPosition({ effectX,playerStartY + (rowHeight + rowGap) * 2 });
	playerEfSiege.setFillColor(sf::Color(0, 0, 0, 0));

	passButton.setSize({ 280.f,90.f });
	passButton.setPosition({panelWidth * 0.15f,	screenHeight - 200.f});
	passButton.setFillColor(sf::Color(140, 40, 40));
	passButton.setOutlineThickness(2.f);
	passButton.setOutlineColor(sf::Color::White);

	exitButton.setSize({ 200.f, 70.f });
	exitButton.setPosition({screenWidth - 250.f,30.f });
	exitButton.setFillColor(sf::Color(90, 30, 30));
	exitButton.setOutlineThickness(2.f);
	exitButton.setOutlineColor(sf::Color::White);

	fineGame.setSize({ screenWidth, screenHeight });
	fineGame.setPosition({ 0.f, 0.f });
	fineGame.setFillColor(sf::Color(0, 0, 0, 180));

	menuButton.setSize({ 360.f, 90.f });
	menuButton.setPosition({ screenWidth / 2.f - 180.f, screenHeight / 2.f + 40.f });
	menuButton.setFillColor(sf::Color(50, 120, 50));
	menuButton.setOutlineThickness(3.f);
	menuButton.setOutlineColor(sf::Color::White);
}
void Board::Update()
{
	if (player.HasPassed() && !bot.HasPassed()&&botClock.getElapsedTime().asSeconds()>1.f)
	{
		bot.MakeMove(player.TotalP(*this), *this, player.HasPassed());
		botClock.restart();
		CheckEnd();
	}
	if (player.HandSize() == 0&&!player.HasPassed())
		player.Pass();

	if (bot.HandSize() == 0 && !bot.HasPassed())
		bot.Pass();
	CheckEnd();

}
void Board::Draw(sf::RenderWindow& window)
{
	window.draw(*boardBackground);
	window.draw(leftPanel);
	window.draw(enemyMelee);
	window.draw(enemyEfMelee);
	window.draw(enemyRanged);
	window.draw(enemyEfRanged);
	window.draw(enemySiege);
	window.draw(enemyEfSiege);
	window.draw(playerMele);
	window.draw(playerEfMele);
	window.draw(playerRanged);
	window.draw(playerEfRanged);
	window.draw(playerSiege);
	window.draw(playerEfSiege);
	player.Draw(window);
	bot.Draw(window);
	DrawWeathO(window);
	if (playerMeleeEff)
		playerMeleeEff->Draw(window);
	if (playerRangedEff)
		playerRangedEff->Draw(window);
	if (playerSiegeEff)
		playerSiegeEff->Draw(window);
	if (enemyMeleeEff)
		enemyMeleeEff->Draw(window);
	if (enemyRangedEff)
		enemyRangedEff->Draw(window);
	if (enemySiegeEff)
		enemySiegeEff->Draw(window);
	DrawDeckDiscard(window);
		auto drawRowPower = [&](int value, float x, float y)
		{
			sf::Text t(resources.GetFont("main"));
			t.setString(std::to_string(value));
			t.setCharacterSize(40);
			t.setFillColor(sf::Color::Yellow);
			t.setOutlineColor(sf::Color::Black);
			t.setOutlineThickness(3.f);
			t.setPosition({ x, y });
			window.draw(t);
		};

	float rx = fieldX - effectWidth - screenWidth * 0.0295f;
	drawRowPower(bot.MeleeP(*this), rx, enemyStartY + 93.f);
	drawRowPower(bot.RangedP(*this), rx, enemyStartY + rowHeight + rowGap + 85.f);
	drawRowPower(bot.SiegeP(*this), rx, enemyStartY + 2.f * (rowHeight + rowGap) + 92.f);

	drawRowPower(player.MeleeP(*this), rx, playerStartY + 85.f);
	drawRowPower(player.RangedP(*this), rx, playerStartY + rowHeight + rowGap + 78.f);
	drawRowPower(player.SiegeP(*this), rx, playerStartY + 2.f * (rowHeight + rowGap) + 83.f);

	drawRowPower(bot.TotalP(*this), 895.f, 635.f);
	drawRowPower(player.TotalP(*this), 895.f, screenHeight - 718.f);

	sf::Text passText(resources.GetFont("main"));
	passText.setCharacterSize(42);
	passText.setFillColor(sf::Color::Yellow);
	passText.setString("Pass");
	passText.setPosition({passButton.getPosition().x + 70.f,passButton.getPosition().y + 18.f});
	window.draw(passButton);
	window.draw(passText);
	if (bot.HasPassed())
	{
		sf::Text botPass(resources.GetFont("main"));
		botPass.setCharacterSize(32);
		botPass.setFillColor(sf::Color(255, 200, 80));
		botPass.setString("BOT PASS");
		botPass.setPosition({ 40.f, 280.f });
		window.draw(botPass);
	}

	if (player.HasPassed())
	{
		sf::Text pPass(resources.GetFont("main"));
		pPass.setCharacterSize(32);
		pPass.setFillColor(sf::Color(255, 200, 80));
		pPass.setString("PASS");
		pPass.setPosition({ 40.f, screenHeight - 400.f });
		window.draw(pPass);
	}
	window.draw(exitButton);
	sf::Text exitText(resources.GetFont("main"));
	exitText.setCharacterSize(36);
	exitText.setFillColor(sf::Color::Yellow);
	exitText.setString("Exit");
	exitText.setPosition({exitButton.getPosition().x + 70.f,exitButton.getPosition().y + 12.f
		});
	window.draw(exitText);
	DrawScorF(window);
	if (weatherFrost) weatherFrost->Draw(window);
	if (weatherFog)   weatherFog->Draw(window);
	if (weatherRain)  weatherRain->Draw(window);
	DrawLive(window);
	if (gameOver)
		DrawEndScreen(window);
}
bool Board::HandleClick(sf::Vector2f mousePos)
{
	if (gameOver)
		return HandleEndClick(mousePos);
	if (exitButton.getGlobalBounds().contains(mousePos))
	{
		gameOver = false;
		resources.StopMusic();
		requestMenu = true;
		return true;
	}
	if (player.HasPassed())
		return false;

	if (passButton.getGlobalBounds().contains(mousePos))
	{
		player.Pass();
		passButton.setFillColor(sf::Color(70, 70, 70));
		bot.MakeMove(player.TotalP(*this), *this, player.HasPassed());
		CheckEnd();
		return true;
	}

	if (player.HandleClick(mousePos))
		return true;
	if (player.PlayDecoy(mousePos, *this))
	{
		bot.MakeMove(player.TotalP(*this), *this, player.HasPassed());
		CheckEnd();
		return true;
	}
	if (player.TryPlayWeather(*this))
	{
		bot.MakeMove(player.TotalP(*this), *this, player.HasPassed());
		CheckEnd();
		return true;
	}
	BoardRow row = GetRowAtPosition(mousePos);
	if (row == BoardRow::None)
		return false;

	if (player.PlayCard(row, *this))
	{
		bot.MakeMove(player.TotalP(*this), *this, player.HasPassed());
		CheckEnd();
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
	if (playerSiege.getGlobalBounds().contains(mousePos)) 
		return BoardRow::PlayerSiege;

	if (enemyMelee.getGlobalBounds().contains(mousePos)) 
		return BoardRow::EnemyMelee;
	if (enemyRanged.getGlobalBounds().contains(mousePos)) 
		return BoardRow::EnemyRanged;
	if (enemySiege.getGlobalBounds().contains(mousePos))  
		return BoardRow::EnemySiege;

	if (playerEfMele.getGlobalBounds().contains(mousePos))  
		return BoardRow::PlayerMelee;
	if (playerEfRanged.getGlobalBounds().contains(mousePos))
		return BoardRow::PlayerRanged;
	if (playerEfSiege.getGlobalBounds().contains(mousePos)) 
		return BoardRow::PlayerSiege;

	if (enemyEfMelee.getGlobalBounds().contains(mousePos))  
		return BoardRow::EnemyMelee;
	if (enemyEfRanged.getGlobalBounds().contains(mousePos))  
		return BoardRow::EnemyRanged;
	if (enemyEfSiege.getGlobalBounds().contains(mousePos))  
		return BoardRow::EnemySiege;

	return BoardRow::None;
}
void Board::BotMove()
{
	bot.MakeMove(player.TotalP(*this), *this, player.HasPassed());
}
// Перевіряє, чи обидва гравці завершили раунд (спасували або
// закінчились карти в руці), і за потреби визначає переможця раунду
// та переможця партії загалом.
void Board::CheckEnd()
{
	if (roundEnd || gameOver)
		return;

	bool playerDone = player.HasPassed() || player.HandSize() == 0;
	bool botDone = bot.HasPassed() || bot.HandSize() == 0;
	if (!playerDone || !botDone)
		return;

	if (player.TotalP(*this) > bot.TotalP(*this))
		bot.LiveLose();
	else if (player.TotalP(*this) < bot.TotalP(*this))
		player.LiveLose();
	// нічия за очками — життя не втрачає ніхто
	if (player.GetLives() <= 0 && bot.GetLives() <= 0)
	{
		gameOver = true;
		isDraw = true;
		playerWin = false;
		stats.RecordDraw(); // фіксуємо нічию в статистиці
		return;
	}
	if (player.GetLives() <= 0)
	{
		gameOver = true;
		isDraw = false;
		playerWin = false;
		stats.RecordLoss(); // фіксуємо поразку гравця
		return;
	}
	if (bot.GetLives() <= 0)
	{
		gameOver = true;
		isDraw = false;
		playerWin = true;
		stats.RecordWin(); // фіксуємо перемогу гравця
		return;
	}
	roundEnd = true;
	NewRound();
}
void Board::NewRound()
{
	player.CleanBoard();
	bot.CleanBoard();
	playerMeleeEff.reset();
	playerRangedEff.reset();
	playerSiegeEff.reset();
	enemyMeleeEff.reset();
	enemyRangedEff.reset();
	enemySiegeEff.reset();
	weatherFrost.reset();
	weatherFog.reset();
	weatherRain.reset();
	weatherM = false;
	weatherR = false;
	weatherS = false;
	hornPlayerMelee = false;
	hornPlayerRanged = false;
	hornPlayerSiege = false;
	hornEnemyMelee = false;
	hornEnemyRanged = false;
	hornEnemySiege = false;
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
void Board::EnableWeather(SpecialType type)
{
	switch (type)
	{
	case SpecialType::Frost:
		weatherM = true;
		break;
	case SpecialType::Fog:
		weatherR = true;
		break;
	case SpecialType::Rain:
		weatherS = true;
		break;
	case SpecialType::ClearWeather:
		weatherM = false;
		weatherR = false;
		weatherS = false;
		break;
	default:
		break;
	}
}
bool Board::IsWeatherM() const
{
	return weatherM;
}
bool Board::IsWeatherR() const
{
	return weatherR;
}
bool Board::IsWeatherS() const
{
	return weatherS;
}
void Board::EnableHorn(BoardRow row)
{
	switch (row)
	{
	case BoardRow::PlayerMelee:
		hornPlayerMelee = true;
		break;
	case BoardRow::PlayerRanged:
		hornPlayerRanged = true;
		break;
	case BoardRow::PlayerSiege:
		hornPlayerSiege = true;
		break;
	case BoardRow::EnemyMelee:
		hornEnemyMelee = true;
		break;
	case BoardRow::EnemyRanged:
		hornEnemyRanged = true;
		break;
	case BoardRow::EnemySiege:
		hornEnemySiege = true;
		break;
	default:
		break;
	}
}
bool Board::IsHorn(BoardRow row) const
{
	switch (row)
	{
	case BoardRow::PlayerMelee:  return hornPlayerMelee;
	case BoardRow::PlayerRanged: return hornPlayerRanged;
	case BoardRow::PlayerSiege:  return hornPlayerSiege;
	case BoardRow::EnemyMelee:   return hornEnemyMelee;
	case BoardRow::EnemyRanged:  return hornEnemyRanged;
	case BoardRow::EnemySiege:   return hornEnemySiege;
	default: return false;
	}
}
void Board::EnableScorch()
{
	ApplyScorch();
	TriggerSE();
}

void Board::ApplyScorch()
{
	int maxPow = std::max(player.MaxUnitPower(*this), bot.MaxUnitPower(*this));
	if (maxPow <= 0) return;
	scorchFlashes.clear();
	auto burnSide = [&](auto& side, bool isPlayer)
		{
			auto rects = isPlayer
				? player.CollectAndRemoveByPower(maxPow, *this)
				: bot.CollectAndRemoveByPower(maxPow, *this);
			for (auto& r : rects)
				scorchFlashes.push_back({ r });
		};

	burnSide(player, true);
	burnSide(bot, false);

	showScorch = !scorchFlashes.empty();
	scorchClock.restart();
}
void Board::ClearWeatherEffects()
{
	auto clearIfWeather = [](std::unique_ptr<Card>& slot)
	{
		if (slot && slot->GetType() == CardType::Weather)
			slot.reset();
	};
	clearIfWeather(playerMeleeEff);
	clearIfWeather(playerRangedEff);
	clearIfWeather(playerSiegeEff);
	clearIfWeather(enemyMeleeEff);
	clearIfWeather(enemyRangedEff);
	clearIfWeather(enemySiegeEff);
}
void Board::SpCardPosition(BoardRow row,std::unique_ptr<Card> card)
{

	switch (row)
	{
	case BoardRow::PlayerMelee:
		playerMeleeEff = std::move(card);
		playerMeleeEff->SetPosition(playerEfMele.getPosition().x, playerEfMele.getPosition().y);
		break;
	case BoardRow::PlayerRanged:
		playerRangedEff = std::move(card);
		playerRangedEff->SetPosition(playerEfRanged.getPosition().x, playerEfRanged.getPosition().y);
		break;
	case BoardRow::PlayerSiege:
		playerSiegeEff = std::move(card);
		playerSiegeEff->SetPosition(playerEfSiege.getPosition().x, playerEfSiege.getPosition().y);
		break;
	case BoardRow::EnemyMelee:
		enemyMeleeEff = std::move(card);
		enemyMeleeEff->SetPosition(enemyEfMelee.getPosition().x, enemyEfMelee.getPosition().y);
		break;
	case BoardRow::EnemyRanged:
		enemyRangedEff = std::move(card);
		enemyRangedEff->SetPosition(enemyEfRanged.getPosition().x, enemyEfRanged.getPosition().y);
		break;
	case BoardRow::EnemySiege:
		enemySiegeEff = std::move(card);
		enemySiegeEff->SetPosition(enemyEfSiege.getPosition().x, enemyEfSiege.getPosition().y);
		break;
	}
}
void Board::DrawLive(sf::RenderWindow& window)
{
	const float targH = 62.f;
	const float gap = 26.f;
	const float lifeX = screenWidth * 0.175f; 
	const float botY = screenHeight * 0.34f; 
	const float plY = screenHeight * 0.62f;
	auto drawGens = [&](int liveCount, float startX, float centerY)
		{
			for (int i=0;i < 2;++i)
			{
				bool alive = (i < liveCount);
				sf::Sprite gem(resources.GetTexture(alive ? "gOn" : "gOff"));
				sf::Vector2u ts = gem.getTexture().getSize();
				float scale = targH / static_cast<float>(ts.y);
				gem.setScale({ scale,scale });
				float w = ts.x * scale;
				gem.setPosition({ startX + i * (w + gap),centerY - targH * 0.5f });
				if (!alive)
					gem.setColor(sf::Color(80, 80, 80, 200));
				window.draw(gem);
			}
		};
	drawGens(bot.GetLives(), lifeX, botY);
	drawGens(player.GetLives(), lifeX, plY);
	
}
void Board::DrawEndScreen(sf::RenderWindow& window)
{
	window.draw(fineGame);
	std::string key = "draw";
	if (!isDraw)
		key = playerWin ? "win" : "lose";
	sf::Sprite banner(resources.GetTexture(key));
	sf::Vector2u ts = banner.getTexture().getSize();
	float scale = (screenWidth * 0.45f) / static_cast<float>(ts.x);
	banner.setScale({ scale, scale });
	float w = ts.x * scale;
	float h = ts.y * scale;
	banner.setPosition({
		(screenWidth - w) * 0.5f,
		(screenHeight - h) * 0.35f
		});
	window.draw(banner);
	window.draw(menuButton);
	sf::Text bttn(resources.GetFont("main"));
	bttn.setCharacterSize(40);
	bttn.setFillColor(sf::Color::White);
	bttn.setString("Menu");
	sf::FloatRect bb = bttn.getLocalBounds();
	bttn.setOrigin({ bb.size.x / 2.f,bb.size.y / 2.f });
	bttn.setPosition({
		menuButton.getPosition().x + menuButton.getSize().x / 2.f,
		menuButton.getPosition().y + menuButton.getSize().y / 2.f
		});
	window.draw(bttn);

	// Рахунок перемог/поразок/нічиїх, збережений у файлі (Statistics)
	sf::Text statsText(resources.GetFont("main"));
	statsText.setCharacterSize(22);
	statsText.setFillColor(sf::Color::White);
	statsText.setString("Wins: " + std::to_string(stats.GetWins()) +
		"   Losses: " + std::to_string(stats.GetLosses()) +
		"   Draws: " + std::to_string(stats.GetDraws()));
	sf::FloatRect sb = statsText.getLocalBounds();
	statsText.setOrigin({ sb.size.x / 2.f, sb.size.y / 2.f });
	statsText.setPosition({ screenWidth * 0.5f, screenHeight * 0.82f });
	window.draw(statsText);
}
bool Board::HandleEndClick(sf::Vector2f mousePos)
{
	if (!gameOver)
		return false;
	if (menuButton.getGlobalBounds().contains(mousePos))
	{
		ResetMatch();
		return true;
	}
	return true;
}
void Board::ResetMatch()
{
	gameOver = false;
	playerWin = false;
	roundEnd = false;
	isDraw = false;
	weatherM = weatherR = weatherS = false;
	hornPlayerMelee = hornPlayerRanged = hornPlayerSiege = false;
	hornEnemyMelee = hornEnemyRanged = hornEnemySiege = false;

	weatherFrost.reset();
	weatherFog.reset();
	weatherRain.reset();

	player.FullReset(resources);
	bot.FullReset(resources);

	player.handY = handY;
	player.TestHand();

	passButton.setFillColor(sf::Color(140, 40, 40));
}
void Board::DrawWeathO(sf::RenderWindow& window)
{
	auto drawOnRow = [&](const std::string& texKey, float y)
		{
			sf::Sprite sp(resources.GetTexture(texKey));
			sf::Vector2u ts = sp.getTexture().getSize();
			float x = fieldX;
			float w = fieldWidth;
			float h = rowHeight;
			sp.setScale({ w / static_cast<float>(ts.x),h / static_cast<float>(ts.y) });
			sp.setPosition({ x,y });
			sp.setColor(sf::Color(255, 255, 255, 245));
			window.draw(sp);
		};
	if (weatherM)
	{
		drawOnRow("frost", enemyStartY + 2.f * (rowHeight + rowGap));
		drawOnRow("frost", playerStartY);
	}
	if (weatherR)
	{
		drawOnRow("fog", enemyStartY + rowHeight + rowGap);
		drawOnRow("fog", playerStartY + rowHeight + rowGap);
	}
	if (weatherS)
	{
		drawOnRow("rain", enemyStartY);
		drawOnRow("rain", playerStartY + 2.f * (rowHeight + rowGap));
	}
}
void Board::TriggerSE()
{
	showScorch = true;
	scorchClock.restart();
}
void Board::DrawScorF(sf::RenderWindow& window)
{
	if (!showScorch) return;

	float t = scorchClock.getElapsedTime().asSeconds();
	if (t > 0.55f)
	{
		showScorch = false;
		scorchFlashes.clear();
		return;
	}

	std::uint8_t alpha = static_cast<std::uint8_t>(255 * (1.f - t / 0.55f));
	sf::Texture& tex = resources.GetTexture("scorch");

	for (auto& f : scorchFlashes)
	{
		sf::Sprite sp(tex);
		auto ts = tex.getSize();
		sp.setScale({
			f.rect.size.x / float(ts.x), 
			f.rect.size.y / float(ts.y)
			});
		sp.setPosition({ f.rect.position.x, f.rect.position.y });
		sp.setColor(sf::Color(255, 255, 255, alpha));
		window.draw(sp);
	}
}
sf::Vector2f Board::WeatherSlotPos(SpecialType st) const
{
	const float startX = screenWidth * 0.09f;
	const float y = screenHeight * 0.42f;
	const float stepX = 130.f;

	switch (st)
	{
	case SpecialType::Frost: return { startX,            y };
	case SpecialType::Fog:   return { startX + stepX,    y };
	case SpecialType::Rain:  return { startX + stepX * 2.f, y };
	default:                 return { startX + stepX,    y };
	}
}
void Board::PlaceWeatherCard(std::unique_ptr<Card> card)
{
	SpecialType st = card->GetSpecialType();
	auto pos = WeatherSlotPos(st);
	card->SetPosition(pos.x, pos.y);

	switch (st)
	{
	case SpecialType::Frost:
		weatherFrost = std::move(card);
		break;
	case SpecialType::Fog:
		weatherFog = std::move(card);
		break;
	case SpecialType::Rain:
		weatherRain = std::move(card);
		break;
	case SpecialType::ClearWeather:
		weatherFrost.reset();
		weatherFog.reset();
		weatherRain.reset();
		ClearWeatherEffects();
		break;
	default:
		break;
	}
}
void Board::DrawDeckDiscard(sf::RenderWindow& window)
{
	const float targetH = 280.f;
	auto drawBack = [&](float x, float y, int count)
		{
			if (count <= 0)
				return;
			sf::Sprite sp(resources.GetTexture("back"));
			sf::Vector2u ts = sp.getTexture().getSize();
			float scale = targetH / static_cast<float>(ts.y);
			sp.setScale({ scale, scale });
			sp.setPosition({ x,y });
			window.draw(sp);
			if (count > 1)
			{
				sp.setPosition({ x + 4.f,y + 4.f });
				window.draw(sp);
			}
			sf::Text n(resources.GetFont("main"));
			n.setCharacterSize(30);
			n.setFillColor(sf::Color::Yellow);
			n.setOutlineColor(sf::Color::Black);
			n.setOutlineThickness(2.f);
			n.setString(std::to_string(count));

			sf::FloatRect b = n.getLocalBounds();
			n.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
			n.setPosition({
				x + ts.x * scale * 0.5f,
				y + targetH * 0.5f
				});
			window.draw(n);
		};
	
	
	float botDeckX = screenWidth * 0.902f;
	float botDeckY = screenHeight * 0.069f;
	float botDiscX = screenWidth * 0.807f;
	float botDiscY = screenHeight * 0.069f;

	float plDeckX = screenWidth * 0.902f;
	float plDeckY = screenHeight * 0.77f;
	float plDiscX = screenWidth * 0.807f;
	float plDiscY = screenHeight * 0.77f;

	drawBack(botDeckX, botDeckY, bot.DeckSize());
	drawBack(botDiscX, botDiscY, bot.DiscardSize());

	drawBack(plDeckX, plDeckY, player.DeckSize());
	drawBack(plDiscX, plDiscY, player.DiscardSize());
}