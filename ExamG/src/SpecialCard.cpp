#include "../include/SpecialCard.h"
#include "../include/Board.h"
SpecialCard::SpecialCard(const sf::Font& font, const std::string& name, SpecialType type)
	: Card(font, name, 0), type(type)
{

}
void SpecialCard::Activate()
{

}
void SpecialCard::Play(Board& board)
{
	switch (type)
	{
	case SpecialType::Weather:
		board.EnableWeather();
		break;
	case SpecialType::Horn:
		board.EnableHorn();
		break;
	case SpecialType::Decoy:
		board.EnableDecoy();
		break;
	case SpecialType::Scorch:
		board.EnableScorch();
		break;
	}
}
CardType SpecialCard::GetType() const
{
	return CardType::Special;
}
int SpecialCard::GetCurPow(const Board&) const
{
	return 0;
}