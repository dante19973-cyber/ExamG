#include "../include/Deck.h"
#include "../include/UnitCard.h"
#include "../include/SpecialCard.h"
#include "../include/Hero.h"
#include "../include/ResourceManager.h"
#include <algorithm>
#include <cstdlib>

// Наповнює колоду фіксованим набором карт: 4 герої, набір юнітів
// та повний комплект спецкарт (погода, ріг, підміна, спалення).
Deck::Deck(const sf::Font& font, ResourceManager& res)
{
	cards.push_back(std::make_unique<Hero>("Geralt", 15, font, res, "geralt"));
	cards.push_back(std::make_unique<Hero>("Ciri", 15, font, res, "ciri"));
	cards.push_back(std::make_unique<Hero>("WinRar", 99, font, res, "winrar"));
	cards.push_back(std::make_unique<Hero>("Yennefer", 7, font, res, "yennefer"));

	cards.push_back(std::make_unique<UnitCard>("Triss", 7, font, res, "triss"));
	cards.push_back(std::make_unique<UnitCard>("Vesemir", 6, font, res, "vesemir"));
	cards.push_back(std::make_unique<UnitCard>("Eskel", 6, font, res, "eskel"));
	cards.push_back(std::make_unique<UnitCard>("Lambert", 6, font, res, "lambert"));
	cards.push_back(std::make_unique<UnitCard>("Zoltan", 5, font, res, "zoltan"));
	cards.push_back(std::make_unique<UnitCard>("Roche", 10, font, res, "roche"));
	cards.push_back(std::make_unique<UnitCard>("Dandelion", 2, font, res, "dandelion"));
	cards.push_back(std::make_unique<UnitCard>("Keira", 5, font, res, "keira"));
	cards.push_back(std::make_unique<UnitCard>("Letho", 10, font, res, "letho"));
	cards.push_back(std::make_unique<UnitCard>("Iorveth", 10, font, res, "iorveth"));
	cards.push_back(std::make_unique<UnitCard>("Thaler", 1, font, res, "thaler"));
	cards.push_back(std::make_unique<UnitCard>("Regis", 10, font, res, "regis"));
	cards.push_back(std::make_unique<UnitCard>("Crach", 11, font, res, "crach"));
	cards.push_back(std::make_unique<UnitCard>("Hjalmar", 10, font, res, "hjalmar"));
	cards.push_back(std::make_unique<UnitCard>("Olgierd", 12, font, res, "olgierd"));
	cards.push_back(std::make_unique<UnitCard>("Blueboy", 8, font, res, "blueboy"));
	cards.push_back(std::make_unique<UnitCard>("Morkvarg", 9, font, res, "morkvarg"));
	cards.push_back(std::make_unique<UnitCard>("Philippa", 8, font, res, "philippa"));
	cards.push_back(std::make_unique<UnitCard>("Donar", 9, font, res, "donar"));

	cards.push_back(std::make_unique<SpecialCard>("Frost", CardType::Weather, SpecialType::Frost, font, res, "frostC"));
	cards.push_back(std::make_unique<SpecialCard>("Fog", CardType::Weather, SpecialType::Fog, font, res, "fogC"));
	cards.push_back(std::make_unique<SpecialCard>("Rain", CardType::Weather, SpecialType::Rain, font, res, "rainC"));
	cards.push_back(std::make_unique<SpecialCard>("Sunny", CardType::Weather, SpecialType::ClearWeather, font, res, "sunny"));
	cards.push_back(std::make_unique<SpecialCard>("Decoy", CardType::Decoy, SpecialType::Decoy, font, res, "decoy"));
	cards.push_back(std::make_unique<SpecialCard>("Horn", CardType::Horn, SpecialType::Horn, font, res, "hornC"));
	cards.push_back(std::make_unique<SpecialCard>("Scorch", CardType::Scorch, SpecialType::Scorch, font, res, "scorchC"));
}

// Проста тасовка Фішера — Йетса на базі rand().
void Deck::Shuffle()
{
	for (size_t i = 0; i < cards.size(); ++i)
	{
		size_t j = static_cast<size_t>(rand()) % cards.size();
		std::swap(cards[i], cards[j]);
	}
}

std::unique_ptr<Card> Deck::GetCard()
{
	if (cards.empty())
		return nullptr;

	auto card = std::move(cards.back());
	cards.pop_back();
	return card;
}

bool Deck::IsEmpty() const
{
	return cards.empty();
}

int Deck::GetSize() const
{
	return static_cast<int>(cards.size());
}
