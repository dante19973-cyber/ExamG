#include "../include/Menu.h"
#include "../include/ResourceManager.h"

// Ініціалізує фон і розташування кнопок меню відносно розміру екрана.
void Menu::Init(ResourceManager& resources, float screenW, float screenH)
{
	this->background = std::make_unique<sf::Sprite>(resources.GetTexture("menu"));
	ResourceManager::FitSprite(*this->background, screenW, screenH);

	const float btnW = 630.f;
	const float btnH = 200.f;
	const float marginRight = 560.f;
	const float cx = screenW - btnW - marginRight;
	const float cy = screenH / 2.f - btnH - 500.f;

	this->playButton = Button(resources.GetFont("main"), "Play", sf::Vector2f{ btnW, btnH }, sf::Vector2f{ cx, cy });
	this->exitButton = Button(resources.GetFont("main"), "Exit", sf::Vector2f{ btnW, btnH }, sf::Vector2f{ cx, cy + 300.f });
	this->ready = true;
}

// У меню немає анімацій чи логіки, що змінюється щокадру — метод залишено
// для єдиного інтерфейсу зі станами Game (Update/Draw/HandleClick).
void Menu::Update()
{
}

void Menu::Draw(sf::RenderWindow& window)
{
	if (!this->ready)
		return;

	if (this->background)
		window.draw(*this->background);

	this->playButton.Draw(window);
	this->exitButton.Draw(window);
}

int Menu::HandleClick(sf::Vector2f mousePos)
{
	if (!this->ready)
		return 0;
	if (this->playButton.IsPressed(mousePos))
		return 1;
	if (this->exitButton.IsPressed(mousePos))
		return 2;
	return 0;
}
