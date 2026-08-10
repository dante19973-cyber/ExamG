#pragma once
#include "Card.h"

class ResourceManager;

// Карта-герой: на відміну від звичайного юніта, не піддається дії
// погоди та рогу — її сила завжди стала.
class Hero : public Card
{
public:
	Hero(const std::string& name, int power,
		const sf::Font& font, ResourceManager& res,
		const std::string& artKey);
	Hero(const sf::Font& font, const std::string& name, int power);

	void Play(Board& board, BoardRow row) override;
	int GetCurPow(const Board& board, BoardRow row) const override;
	CardType GetType() const override;
};
