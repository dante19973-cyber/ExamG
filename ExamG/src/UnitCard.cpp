#include "../include/UnitCard.h"
#include "../include/Board.h"
UnitCard::UnitCard(const sf::Font& font, const std::string& name, int power)
	:Card(font, name, power)
{

}
void UnitCard::Play(Board& board)
{

}
CardType UnitCard::GetType() const
{
	return CardType::Unit;
}
int UnitCard::GetCurPow(const Board& board) const
{
	int value = GetPower();
	if (board.IsWeatherAct())
		return 1;
	return value;
}