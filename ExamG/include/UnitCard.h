#pragma once
#include "Card.h"

class ResourceManager;

// Звичайний юніт. Його поточна сила залежить від погоди та рогу
// в тому ряду, куди його викладено — це рахує GetCurPow().
class UnitCard : public Card
{
public:
	// Повний конструктор: одразу підвантажує арт через ResourceManager.
	UnitCard(const std::string& name, int power,
		const sf::Font& font, ResourceManager& res,
		const std::string& artKey);

	// Спрощений конструктор без арту (для тестів/заглушок).
	UnitCard(const sf::Font& font, const std::string& name, int power);

	void Play(Board& board, BoardRow row) override;
	CardType GetType() const override;
	int GetCurPow(const Board& board, BoardRow row) const override;
};
