#pragma once
#include <vector>
#include <memory>
#include "Card.h"

class ResourceManager;

// Колода карт одного гравця. Створюється повністю заповненою
// фіксованим набором карт (герої, юніти, спецкарти), а далі
// картки просто виймаються з неї по одній (як зі стосу).
class Deck
{
	std::vector<std::unique_ptr<Card>> cards;

public:
	Deck() = default;
	Deck(const sf::Font& font, ResourceManager& res);

	void Shuffle();

	// Забирає верхню карту колоди (переміщує володіння викликачу).
	// Повертає nullptr, якщо колода порожня.
	std::unique_ptr<Card> GetCard();

	bool IsEmpty() const;
	int GetSize() const;
};
