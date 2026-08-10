#include "../include/SpecialCard.h"
#include "../include/ResourceManager.h"
#include "../include/Board.h"

SpecialCard::SpecialCard(const sf::Font& font, const std::string& name, SpecialType st)
	: Card(font, name, 0), type(st)
{
}

SpecialCard::SpecialCard(const std::string& name,
	CardType cardType,
	SpecialType st,
	const sf::Font& font,
	ResourceManager& res,
	const std::string& artKey)
	: Card(font, name, 0), type(st)
{
	nameText.setCharacterSize(14);
	nameText.setFillColor(sf::Color::White);
	powerText.setString(""); // у спецкарт немає числової сили — ховаємо підпис
	SetVis(res, artKey, cardType, st);
}

// Застосовує ефект карти до дошки залежно від її підтипу.
// Це і є поліморфна "поведінка" карти при виході на поле.
void SpecialCard::Play(Board& board, BoardRow row)
{
	switch (type)
	{
	case SpecialType::Frost:
	case SpecialType::Fog:
	case SpecialType::Rain:
	case SpecialType::ClearWeather:
		board.EnableWeather(type);
		break;
	case SpecialType::Horn:
		board.EnableHorn(row);
		break;
	case SpecialType::Decoy:
		// Логіка підміни (повернення юніта в руку) обробляється
		// окремо в Player/Bot, тут додаткова дія не потрібна.
		break;
	case SpecialType::Scorch:
		board.EnableScorch();
		break;
	default:
		break;
	}
}

CardType SpecialCard::GetType() const
{
	switch (type)
	{
	case SpecialType::Frost:
	case SpecialType::Fog:
	case SpecialType::Rain:
	case SpecialType::ClearWeather:
		return CardType::Weather;
	case SpecialType::Horn:
		return CardType::Horn;
	case SpecialType::Decoy:
		return CardType::Decoy;
	case SpecialType::Scorch:
		return CardType::Scorch;
	default:
		return CardType::Weather;
	}
}

// Спецкарти не мають числової сили на полі.
int SpecialCard::GetCurPow(const Board&, BoardRow) const
{
	return 0;
}

SpecialType SpecialCard::GetSpecialType() const
{
	return type;
}
