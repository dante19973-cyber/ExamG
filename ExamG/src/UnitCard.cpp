#include "../include/UnitCard.h"
#include "../include/ResourceManager.h"
#include "../include/Board.h"

UnitCard::UnitCard(const sf::Font& font, const std::string& name, int power)
	: Card(font, name, power)
{
}

UnitCard::UnitCard(const std::string& name, int power,
	const sf::Font& font, ResourceManager& res,
	const std::string& artKey)
	: Card(font, name, power)
{
	SetVis(res, artKey, CardType::Unit, SpecialType::None);
}

// Юніт сам по собі не має спецефекту при виході на поле —
// уся його "поведінка" зводиться до підрахунку сили в GetCurPow().
void UnitCard::Play(Board&, BoardRow)
{
}

CardType UnitCard::GetType() const
{
	return CardType::Unit;
}

// Рахує поточну силу юніта з урахуванням погоди (знижує силу до 1)
// та рогу (подвоює силу) у тому ряду, де стоїть карта.
int UnitCard::GetCurPow(const Board& board, BoardRow row) const
{
	int value = GetPower();

	if (row == BoardRow::PlayerMelee || row == BoardRow::EnemyMelee)
	{
		if (board.IsWeatherM())
			value = 1;
		if (board.IsHorn(row))
			value *= 2;
	}
	if (row == BoardRow::PlayerRanged || row == BoardRow::EnemyRanged)
	{
		if (board.IsWeatherR())
			value = 1;
		if (board.IsHorn(row))
			value *= 2;
	}
	if (row == BoardRow::PlayerSiege || row == BoardRow::EnemySiege)
	{
		if (board.IsWeatherS())
			value = 1;
		if (board.IsHorn(row))
			value *= 2;
	}
	return value;
}
