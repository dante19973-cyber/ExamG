#pragma once
#include <SFML/Graphics.hpp>
#include "Card.h"
#include "Player.h"
#include "Bot.h"
#include "BoardRow.h"
#include "SpecialCard.h"
#include "Statistics.h"
#include <vector>
#include <memory>

class ResourceManager;

// Головний клас гри: тримає гравця й бота, ігрове поле (координати
// рядів), усі активні ефекти (погода, ріг, спалення) та стан партії
// (раунд/матч завершено чи ні). Це "диригент", який пов'язує всі інші
// класи (Player, Bot, Card, Statistics) в одну партію Ґвінта.
class Board
{
	Player player;
	Bot bot;
	bool roundEnd = false;

	// Геометрія поля — рахується один раз у конструкторі відносно
	// роздільної здатності екрана, щоб інтерфейс масштабувався.
	float screenWidth;
	float screenHeight;
	float panelWidth;
	float fieldX;
	float fieldWidth;
	float rowHeight;
	float rowGap;
	float effectWidth;
	float enemyStartY;
	float playerStartY;
	float handY;
	float scoreX;
	ResourceManager& resources;
	std::unique_ptr<sf::Sprite> boardBackground;

	// Прямокутники-"кнопки" рядів поля (для кліків) і сусідні з ними
	// прямокутники слотів під спецкарти (Ef = "ефект").
	sf::RectangleShape playerMele;
	sf::RectangleShape playerEfMele;
	sf::RectangleShape playerRanged;
	sf::RectangleShape playerEfRanged;
	sf::RectangleShape playerSiege;
	sf::RectangleShape playerEfSiege;
	sf::RectangleShape enemyMelee;
	sf::RectangleShape enemyEfMelee;
	sf::RectangleShape enemyRanged;
	sf::RectangleShape enemyEfRanged;
	sf::RectangleShape enemySiege;
	sf::RectangleShape enemyEfSiege;

	// Спецкарти (ріг/спалення), що лежать поруч зі своїм рядом.
	std::unique_ptr<Card> playerMeleeEff;
	std::unique_ptr<Card> playerRangedEff;
	std::unique_ptr<Card> playerSiegeEff;
	std::unique_ptr<Card> enemyMeleeEff;
	std::unique_ptr<Card> enemyRangedEff;
	std::unique_ptr<Card> enemySiegeEff;

	// Активні погодні карти (спільні для обох гравців).
	std::unique_ptr<Card> weatherFrost;
	std::unique_ptr<Card> weatherFog;
	std::unique_ptr<Card> weatherRain;

	sf::RectangleShape passButton;
	sf::RectangleShape leftPanel;
	sf::RectangleShape menuButton;
	sf::RectangleShape fineGame;
	sf::RectangleShape exitButton;

	sf::Clock botClock;    // затримка перед ходом бота, щоб хід не був миттєвим
	sf::Clock scorchClock; // тривалість анімації спалахів "Спалення"

	// Прямокутник, у якому щойно згорів юніт — потрібен лише для анімації.
	struct ScorchFlash
	{
		sf::FloatRect rect;
	};
	std::vector<ScorchFlash> scorchFlashes;
	bool showScorch = false;

	// Прапорці активних ефектів поля.
	bool weatherM = false;
	bool weatherR = false;
	bool weatherS = false;
	bool hornPlayerMelee = false;
	bool hornPlayerRanged = false;
	bool hornPlayerSiege = false;
	bool hornEnemyMelee = false;
	bool hornEnemyRanged = false;
	bool hornEnemySiege = false;

	bool gameOver = false;
	bool playerWin = false;
	bool isDraw = false;
	bool requestMenu = false;

	Statistics stats; // збережений рахунок перемог/поразок (файл gwent_stats.txt)

public:
	Board(ResourceManager& resources);

	// Аксесори геометрії поля — ними користуються Player/Bot, щоб
	// розкладати карти рівно у своїх рядах.
	float GetPlayerStartY() const { return playerStartY; }
	float GetEnemyStartY() const { return enemyStartY; }
	float GetFieldX() const { return fieldX; }
	float GetRowHeight() const { return rowHeight; }
	float GetRowGap() const { return rowGap; }
	float GetFieldWidth() const { return fieldWidth; }

	void Update();
	void Draw(sf::RenderWindow& window);
	void BotMove();
	bool HandleClick(sf::Vector2f mousePos);
	BoardRow GetRowAtPosition(sf::Vector2f mousePos) const;

	void CheckEnd();
	void NewRound();
	bool IsOver() const;

	// Погода.
	void EnableWeather(SpecialType type);
	bool IsWeatherM() const;
	bool IsWeatherR() const;
	bool IsWeatherS() const;

	// Ріг.
	void EnableHorn(BoardRow row);
	bool IsHorn(BoardRow row) const;

	// Спалення.
	void EnableScorch();
	void ApplyScorch();
	void TriggerSE();
	void DrawScorF(sf::RenderWindow& window);

	void DrawWeathO(sf::RenderWindow& window);
	void SpCardPosition(BoardRow row, std::unique_ptr<Card> card);
	void ClearWeatherEffects();
	void DrawLive(sf::RenderWindow& window);

	// Завершення партії.
	void DrawEndScreen(sf::RenderWindow& window);
	bool HandleEndClick(sf::Vector2f mousePos);
	bool IsGameOver() const { return gameOver; }
	bool PlayerWon() const { return playerWin; }
	void ResetMatch();

	bool WantMenu() const { return requestMenu; }
	void ClearMenuRequest() { requestMenu = false; }

	sf::Vector2f WeatherSlotPos(SpecialType st) const;
	void PlaceWeatherCard(std::unique_ptr<Card> card);
	void DrawDeckDiscard(sf::RenderWindow& window);

	// Аксесори для статистики матчів.
	int GetWins() const { return stats.GetWins(); }
	int GetLosses() const { return stats.GetLosses(); }
	int GetDraws() const { return stats.GetDraws(); }
};
