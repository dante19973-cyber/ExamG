#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>

// Централізовано завантажує й зберігає всі ресурси гри (шрифти,
// текстури, музику), щоб кожен клас звертався по них за ключем-рядком,
// а не тримав власні копії файлів.
class ResourceManager
{
	std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Font> fonts;
	sf::Music battleMusic;

	void LoadFonts();
	void LoadTextures();
	void LoadSounds();

public:
	ResourceManager() = default;

	void LoadResources();

	sf::Texture& GetTexture(const std::string& name);
	sf::Font& GetFont(const std::string& name);

	void PlayMusic();
	void StopMusic();

	// Масштабує й центрує спрайт так, щоб він повністю вписався
	// в екран, зберігаючи пропорції.
	static void FitSprite(sf::Sprite& sp, float screenW, float screenH);
};
