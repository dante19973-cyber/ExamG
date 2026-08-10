#pragma once
#include <string>
#include "Card.h"
#include "CardType.h"

class Board;
class ResourceManager;

// Спеціальна карта: погода, ріг, підміна або спалення.
// На відміну від юніта/героя, вона не має "сирої" сили — її роль
// полягає в тому, щоб застосувати ефект до дошки через Play().
class SpecialCard : public Card
{
	SpecialType type;

public:
	SpecialCard(const std::string& name,
		CardType cardType,
		SpecialType st,
		const sf::Font& font,
		ResourceManager& res,
		const std::string& artKey);
	SpecialCard(const sf::Font& font, const std::string& name, SpecialType st);

	void Play(Board& board, BoardRow row) override;
	CardType GetType() const override;
	int GetCurPow(const Board& board, BoardRow row) const override;
	SpecialType GetSpecialType() const override;
};
