#include "../include/Deck.h"
#include "../include/UnitCard.h"
#include "../include/SpecialCard.h"
#include<algorithm>
#include<random>
Deck::Deck()
	:currentSize(0)
{

}
Deck::Deck(const sf::Font& font)
{
	cards.push_back(std::make_unique<UnitCard>(font, "Geralt", 15));
	cards.push_back(std::make_unique<UnitCard>(font, "Ciri", 15));
	cards.push_back(std::make_unique<UnitCard>(font, "Yennefer", 7));
	cards.push_back(std::make_unique<UnitCard>(font, "Triss", 7));
	cards.push_back(std::make_unique<UnitCard>(font, "Vesemir", 6));
	cards.push_back(std::make_unique<UnitCard>(font, "Eskel", 6));
	cards.push_back(std::make_unique<UnitCard>(font, "Lambert", 6));
	cards.push_back(std::make_unique<UnitCard>(font, "Zoltan", 5));
	cards.push_back(std::make_unique<UnitCard>(font, "Roche", 10));
	cards.push_back(std::make_unique<UnitCard>(font, "Dandelion", 2));
	cards.push_back(std::make_unique<UnitCard>(font, "Keira", 5));
	cards.push_back(std::make_unique<UnitCard>(font, "Letho", 10));
	cards.push_back(std::make_unique<UnitCard>(font, "Iorveth", 10));
	cards.push_back(std::make_unique<UnitCard>(font, "Thaler", 1));
	cards.push_back(std::make_unique<UnitCard>(font, "Vernon", 10));
	cards.push_back(std::make_unique<UnitCard>(font, "Crach", 11));
	cards.push_back(std::make_unique<UnitCard>(font, "Hjalmar", 10));
	cards.push_back(std::make_unique<UnitCard>(font, "Olaf", 12));
	cards.push_back(std::make_unique<UnitCard>(font, "Blueboy", 8));
	cards.push_back(std::make_unique<UnitCard>(font, "Morkvarg", 9));
	cards.push_back(std::make_unique<UnitCard>(font, "Birna", 8));
	cards.push_back(std::make_unique<UnitCard>(font, "Donar", 9));
	cards.push_back(std::make_unique<SpecialCard>(font,"Biting Frost",SpecialType::Weather));
	cards.push_back(std::make_unique<SpecialCard>(font,"Decoy",SpecialType::Decoy));
	cards.push_back(std::make_unique<SpecialCard>(font,"Commander's Horn",SpecialType::Horn));
	cards.push_back(std::make_unique<SpecialCard>(font, "Biting Fire", SpecialType::Scorch));

}
void Deck::Shuffle()
{
	for (size_t i = 0;i < cards.size();i++)
	{
		size_t j = rand() % cards.size();
		std::swap(cards[i], cards[j]);
	}
}
std::unique_ptr<Card>Deck::GetCard()
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
	return cards.size();
}
void Deck::AddCard()
{

}
