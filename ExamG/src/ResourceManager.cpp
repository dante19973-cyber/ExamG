#include "../include/ResourceManager.h"
#include <stdexcept>
ResourceManager::ResourceManager()
{

}
void ResourceManager::LoadResources()
{
	LoadFonts();
	LoadTextures();
	LoadSounds();
}
void ResourceManager::LoadFonts()
{
		sf::Font font;
		if (!font.openFromFile("assets/fonts/gwent_extrabold.ttf"))
		{
			throw std::runtime_error("Error font file");
		}
		fonts["main"] = std::move(font);

}
void ResourceManager::LoadTextures()
{

}
void ResourceManager::LoadSounds() {

}
sf::Texture& ResourceManager::GetTexture(const std::string& name)
{
	return textures.at(name);
}
sf::Font& ResourceManager::GetFont(const std::string& name)
{
	return fonts.at(name);
}