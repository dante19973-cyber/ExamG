#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include <memory>

class ResourceManager;

// Головне меню гри: фон + дві кнопки ("Play" і "Exit").
class Menu
{
	std::unique_ptr<sf::Sprite> background;
	Button playButton;
	Button exitButton;
	bool ready = false; // true після Init(), поки не готове — нічого не малюємо

public:
	Menu() = default;

	void Init(ResourceManager& resources, float screenW, float screenH);
	void Update();
	void Draw(sf::RenderWindow& window);

	// Повертає 0 — нічого не натиснуто, 1 — "Play", 2 — "Exit".
	int HandleClick(sf::Vector2f mousePos);
};
