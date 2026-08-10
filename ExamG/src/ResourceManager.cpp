#include "../include/ResourceManager.h"
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <utility>

// Завантажує всі ресурси гри в правильному порядку: шрифти -> текстури -> звук.
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

// Список усіх текстур гри: ключ, за яким її потім шукатимуть класи,
// та шлях до файлу на диску.
void ResourceManager::LoadTextures()
{
	static const std::vector<std::pair<std::string, std::string>> textureFiles = {
		{ "splash", "assets/backgrounds/Gwent_logo.png" },
		{ "menu", "assets/backgrounds/Untitled_Artwork.png" },
		{ "board", "assets/backgrounds/board.png" },
		{ "gOn", "assets/texture/gem_on.png" },
		{ "gOff", "assets/texture/gem_off.png" },
		{ "draw", "assets/texture/draw.png" },
		{ "win", "assets/texture/win.png" },
		{ "lose", "assets/texture/lose.png" },
		{ "frost", "assets/texture/overlay_frost.png" },
		{ "fog", "assets/texture/overlay_fog.png" },
		{ "rain", "assets/texture/overlay_rain.png" },
		{ "scorch", "assets/texture/anim_scorch.png" },
		{ "back", "assets/texture/back.png" },
		{ "blueboy", "assets/cards/Blueboy.png" },
		{ "ciri", "assets/cards/Ciri.png" },
		{ "sunny", "assets/cards/Clear.png" },
		{ "crach", "assets/cards/Crach.png" },
		{ "dandelion", "assets/cards/Dandelion.png" },
		{ "decoy", "assets/cards/Decoy.png" },
		{ "donar", "assets/cards/Donar.png" },
		{ "eskel", "assets/cards/Eskel.png" },
		{ "fogC", "assets/cards/Fog.png" },
		{ "frostC", "assets/cards/Frost.png" },
		{ "geralt", "assets/cards/Geralt.png" },
		{ "hjalmar", "assets/cards/Hjalmar.png" },
		{ "hornC", "assets/cards/Horn.png" },
		{ "iorveth", "assets/cards/Iorveth.png" },
		{ "keira", "assets/cards/Keira.png" },
		{ "lambert", "assets/cards/Lambert.png" },
		{ "letho", "assets/cards/Letho.png" },
		{ "morkvarg", "assets/cards/Morkvarg.png" },
		{ "olgierd", "assets/cards/Olgierd.png" },
		{ "philippa", "assets/cards/Philippa.png" },
		{ "rainC", "assets/cards/Rain.png" },
		{ "regis", "assets/cards/Regis.png" },
		{ "roche", "assets/cards/Roche.png" },
		{ "scorchC", "assets/cards/Scorch.png" },
		{ "thaler", "assets/cards/Thaler.png" },
		{ "triss", "assets/cards/Triss.png" },
		{ "vesemir", "assets/cards/Vesemir.png" },
		{ "yennefer", "assets/cards/Yennefer.png" },
		{ "zoltan", "assets/cards/Zoltan.png" },
		{ "winrar", "assets/cards/winrar.png" },
		{ "frostR", "assets/texture/frost.png" },
		{ "fogR", "assets/texture/fog.png" },
		{ "rainR", "assets/texture/rain.png" },
		{ "scorchR", "assets/texture/scorch.png" },
		{ "sunnyR", "assets/texture/clear.png" },
		{ "hornR", "assets/texture/horn.png" },
		{ "decoyR", "assets/texture/decoy.png" },
		{ "heroR", "assets/texture/power_hero.png" },
		{ "normR", "assets/texture/power_normal.png" },
	};

	for (const auto& [key, path] : textureFiles)
	{
		sf::Texture tex;
		if (!tex.loadFromFile(path))
			throw std::runtime_error("Failed to load texture: " + path);
		textures[key] = std::move(tex);
	}
}

void ResourceManager::LoadSounds()
{
	if (!battleMusic.openFromFile("assets/sounds/gwent_track.wav"))
	{
		throw std::runtime_error("Error sound file");
	}
	battleMusic.setLooping(true);
	battleMusic.setVolume(50.f);
}

sf::Texture& ResourceManager::GetTexture(const std::string& name)
{
	return textures.at(name);
}

sf::Font& ResourceManager::GetFont(const std::string& name)
{
	return fonts.at(name);
}

// Запускає бойову музику з 10-ї секунди (пропускає тиху вступну частину),
// якщо вона ще не грає.
void ResourceManager::PlayMusic()
{
	if (battleMusic.getStatus() != sf::SoundSource::Status::Playing)
	{
		battleMusic.setPlayingOffset(sf::seconds(10.f));
		battleMusic.play();
	}
}

void ResourceManager::StopMusic()
{
	battleMusic.stop();
}

// Вписує спрайт у прямокутник екрана, зберігаючи пропорції,
// і центрує його по обох осях.
void ResourceManager::FitSprite(sf::Sprite& sp, float screenW, float screenH)
{
	const sf::Texture& tex = sp.getTexture();
	sf::Vector2u ts = tex.getSize();
	float texW = static_cast<float>(ts.x);
	float texH = static_cast<float>(ts.y);

	float scale = std::min(screenW / texW, screenH / texH);
	sp.setScale({ scale, scale });

	float w = texW * scale;
	float h = texH * scale;
	sp.setPosition({
		(screenW - w) * 0.5f,
		(screenH - h) * 0.5f
		});
}
