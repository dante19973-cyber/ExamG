#include "../include/Menu.h"
#include <iostream>
Menu::Menu()
{

}
void Menu::Update()
{

}
void Menu::Draw(sf::RenderWindow& window)
{
	playButton.Draw(window);
}
bool Menu::HandleClick(sf::Vector2f mousePos) {
	if (playButton.IsPressed(mousePos))
	{
		std::cout << "\a";
		return true;
	}
	return false;
}