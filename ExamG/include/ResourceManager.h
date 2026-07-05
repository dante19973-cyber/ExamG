#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
class ResourceManager
{
	std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Font> fonts;

	void LoadFonts();
	void LoadTextures();
	void LoadSounds();
public:
	ResourceManager();
	void LoadResources();
	sf::Texture& GetTexture(const std::string& name);
	sf::Font& GetFont(const std::string& name);
};

