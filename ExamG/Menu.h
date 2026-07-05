#pragma once
//#include <SFML/Graphics.hpp>
//#include<vector>
#include "Button.h"
class Menu
{
	/*std::vector<Button> buttons;
	bool isActive;*/
	Button playButton;
public:
	Menu();
	void Update();
	void Draw(sf::RenderWindow& window);
	bool HandleClick(sf::Vector2f mousePos);
};

