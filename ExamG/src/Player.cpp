#include "../include/Player.h"
#include "../include/ResourceManager.h"
#include "../include/Board.h"
#include <algorithm>

// Створює повну колоду, тасує її та роздає стартову руку з 10 карт.
Player::Player(ResourceManager& res)
	: deck(res.GetFont("main"), res)
{
	this->deck.Shuffle();
	for (int i = 0; i < 10; ++i)
	{
		this->hand.push_back(this->deck.GetCard());
	}
	this->TestHand();
}

// Розкладає карти руки в один ряд знизу екрана з рівним кроком.
void Player::TestHand()
{
	const float startX = 1200.f;
	for (size_t i = 0; i < this->hand.size(); ++i)
	{
		this->hand[i]->SetPosition(startX + i * 165.f, this->handY);
	}
}

void Player::AddCard(std::unique_ptr<Card> card)
{
	this->hand.push_back(std::move(card));
}

// Знаходить виділену карту руки й розігрує її в заданий ряд.
// Погодні карти, ріг і спалення обробляються окремо від звичайних юнітів,
// бо йдуть не в один із трьох рядів гравця, а впливають на все поле.
bool Player::PlayCard(BoardRow row, Board& board)
{
	if (this->passed)
		return false;

	for (size_t i = 0; i < this->hand.size(); ++i)
	{
		if (!this->hand[i] || !this->hand[i]->IsSelected())
			continue;

		CardType type = this->hand[i]->GetType();

		// Підміна розігрується окремим методом (PlayDecoy), не тут.
		if (type == CardType::Decoy)
			return false;

		std::unique_ptr<Card> card = std::move(this->hand[i]);
		this->hand.erase(this->hand.begin() + static_cast<long long>(i));

		if (type == CardType::Weather || type == CardType::Horn || type == CardType::Scorch)
		{
			SpecialType st = card->GetSpecialType();

			// Ріг можна класти лише в один зі своїх трьох рядів.
			if (type == CardType::Horn)
			{
				if (row != BoardRow::PlayerMelee &&
					row != BoardRow::PlayerRanged &&
					row != BoardRow::PlayerSiege)
				{
					this->hand.push_back(std::move(card));
					this->TestHand();
					return false;
				}
			}

			// Кожен вид погоди можна класти лише у "свій" ряд
			// (мороз — ближній бій, туман — дальній, дощ — облога);
			// ясна погода та невідомий тип дозволені завжди.
			if (type == CardType::Weather)
			{
				bool allowed;
				if (st == SpecialType::Frost)
					allowed = (row == BoardRow::PlayerMelee || row == BoardRow::EnemyMelee);
				else if (st == SpecialType::Fog)
					allowed = (row == BoardRow::PlayerRanged || row == BoardRow::EnemyRanged);
				else if (st == SpecialType::Rain)
					allowed = (row == BoardRow::PlayerSiege || row == BoardRow::EnemySiege);
				else
					allowed = true;

				if (!allowed)
				{
					this->hand.push_back(std::move(card));
					this->TestHand();
					return false;
				}
			}

			if (type == CardType::Scorch)
			{
				card->Play(board, row);
				this->TestHand();
				return true;
			}

			card->Play(board, row);
			board.SpCardPosition(row, std::move(card));
			this->TestHand();
			return true;
		}

		if (row == BoardRow::PlayerMelee)
		{
			this->meleeRow.push_back(std::move(card));
			this->meleeRow.back()->SetPosition(
				board.GetFieldX() + 20.f + (this->meleeRow.size() - 1) * 165.f,
				board.GetPlayerStartY() + 10.f);
			this->meleeRow.back()->Play(board, row);
			this->TestHand();
			return true;
		}
		else if (row == BoardRow::PlayerRanged)
		{
			this->rangedRow.push_back(std::move(card));
			this->rangedRow.back()->SetPosition(
				board.GetFieldX() + 20.f + (this->rangedRow.size() - 1) * 165.f,
				board.GetPlayerStartY() + board.GetRowHeight() + board.GetRowGap() + 10.f);
			this->rangedRow.back()->Play(board, row);
			this->TestHand();
			return true;
		}
		else if (row == BoardRow::PlayerSiege)
		{
			this->siegeRow.push_back(std::move(card));
			this->siegeRow.back()->SetPosition(
				board.GetFieldX() + 20.f + (this->siegeRow.size() - 1) * 165.f,
				board.GetPlayerStartY() + 2.f * (board.GetRowHeight() + board.GetRowGap()) + 10.f);
			this->siegeRow.back()->Play(board, row);
			this->TestHand();
			return true;
		}

		// Клік поза дозволеними рядами — повертаємо карту в руку.
		this->hand.push_back(std::move(card));
		this->TestHand();
		return true;
	}
	return false;
}

void Player::Draw(sf::RenderWindow& window)
{
	for (const auto& card : this->meleeRow)
		card->Draw(window);
	for (const auto& card : this->rangedRow)
		card->Draw(window);
	for (const auto& card : this->siegeRow)
		card->Draw(window);
	for (const auto& card : this->hand)
		card->Draw(window);
}

// Клік по руці знімає виділення з усіх карт і виділяє ту, під якою клікнули.
bool Player::HandleClick(sf::Vector2f mousePos)
{
	for (auto& card : this->hand)
	{
		if (card->Contains(mousePos))
		{
			for (auto& c : this->hand)
				c->SetSelected(false);
			card->SetSelected(true);
			return true;
		}
	}
	return false;
}

int Player::MeleeP(const Board& board) const
{
	int sum = 0;
	for (const auto& card : this->meleeRow)
		sum += card->GetCurPow(board, BoardRow::PlayerMelee);
	return sum;
}

int Player::RangedP(const Board& board) const
{
	int sum = 0;
	for (const auto& card : this->rangedRow)
		sum += card->GetCurPow(board, BoardRow::PlayerRanged);
	return sum;
}

int Player::SiegeP(const Board& board) const
{
	int sum = 0;
	for (const auto& card : this->siegeRow)
		sum += card->GetCurPow(board, BoardRow::PlayerSiege);
	return sum;
}

int Player::TotalP(const Board& board) const
{
	return this->MeleeP(board) + this->RangedP(board) + this->SiegeP(board);
}

void Player::Pass()
{
	this->passed = true;
}

bool Player::HasPassed() const
{
	return this->passed;
}

void Player::LiveLose()
{
	if (this->lives > 0)
		this->lives--;
}

int Player::GetLives() const
{
	return this->lives;
}

int Player::HandSize() const
{
	return static_cast<int>(this->hand.size());
}

// Переносить усі карти з трьох рядів у скидання й готує гравця
// до наступного раунду (не до нової партії — колода й життя лишаються).
void Player::CleanBoard()
{
	for (auto& c : this->meleeRow)
		if (c) this->AddToDiscard(std::move(c));
	for (auto& c : this->rangedRow)
		if (c) this->AddToDiscard(std::move(c));
	for (auto& c : this->siegeRow)
		if (c) this->AddToDiscard(std::move(c));

	this->meleeRow.clear();
	this->rangedRow.clear();
	this->siegeRow.clear();
	this->passed = false;

	for (auto& card : this->hand)
		card->SetSelected(false);
	this->TestHand();
}

void Player::DrawCard()
{
	if (!this->deck.IsEmpty())
		this->hand.push_back(this->deck.GetCard());
	this->TestHand();
}

// Найбільша сила серед юнітів (герої не рахуються) по всіх трьох рядах —
// саме її знищує карта "Спалення".
int Player::MaxUnitPower(const Board& board) const
{
	int maxPow = 0;
	for (const auto& card : this->meleeRow)
		if (card->GetType() != CardType::Hero)
			maxPow = std::max(maxPow, card->GetCurPow(board, BoardRow::PlayerMelee));
	for (const auto& card : this->rangedRow)
		if (card->GetType() != CardType::Hero)
			maxPow = std::max(maxPow, card->GetCurPow(board, BoardRow::PlayerRanged));
	for (const auto& card : this->siegeRow)
		if (card->GetType() != CardType::Hero)
			maxPow = std::max(maxPow, card->GetCurPow(board, BoardRow::PlayerSiege));
	return maxPow;
}

// Шукає виділену карту "Підміна" в руці та карту на полі під курсором,
// після чого міняє їх місцями (підміна йде на поле, знята карта — в руку).
bool Player::PlayDecoy(sf::Vector2f mousePos, Board& board)
{
	if (this->passed)
		return false;

	int decoyIndex = -1;
	for (size_t i = 0; i < this->hand.size(); ++i)
	{
		if (this->hand[i] && this->hand[i]->IsSelected() && this->hand[i]->GetType() == CardType::Decoy)
		{
			decoyIndex = static_cast<int>(i);
			break;
		}
	}
	if (decoyIndex < 0)
		return false;

	// Пробує виконати підміну в конкретному ряду; повертає true при успіху.
	auto tryRow = [&](std::vector<std::unique_ptr<Card>>& rowCards, float baseY) -> bool
		{
			for (size_t i = 0; i < rowCards.size(); ++i)
			{
				if (!rowCards[i] || !rowCards[i]->Contains(mousePos))
					continue;

				auto removedCard = std::move(rowCards[i]);
				removedCard->SetSelected(false);
				rowCards.erase(rowCards.begin() + i);

				auto decoy = std::move(this->hand[decoyIndex]);
				this->hand.erase(this->hand.begin() + decoyIndex);
				decoy->SetSelected(false);
				rowCards.insert(rowCards.begin() + i, std::move(decoy));

				for (size_t j = 0; j < rowCards.size(); ++j)
					rowCards[j]->SetPosition(board.GetFieldX() + 20.f + j * 165.f, baseY);

				this->hand.push_back(std::move(removedCard));
				this->TestHand();
				return true;
			}
			return false;
		};

	float yMelee = board.GetPlayerStartY() + 10.f;
	float yRanged = board.GetPlayerStartY() + board.GetRowHeight() + board.GetRowGap() + 10.f;
	float ySiege = board.GetPlayerStartY() + 2.f * (board.GetRowHeight() + board.GetRowGap()) + 10.f;

	if (tryRow(this->meleeRow, yMelee))  return true;
	if (tryRow(this->rangedRow, yRanged)) return true;
	if (tryRow(this->siegeRow, ySiege))  return true;

	return false;
}

// Повністю скидає гравця до стану нової партії: нова тасована колода,
// свіжа рука, повні життя, порожні поле й скидання.
void Player::FullReset(ResourceManager& res)
{
	this->hand.clear();
	this->meleeRow.clear();
	this->rangedRow.clear();
	this->siegeRow.clear();
	this->discard.clear();
	this->passed = false;
	this->lives = 2;

	this->deck = Deck(res.GetFont("main"), res);
	this->deck.Shuffle();

	for (int i = 0; i < 10; ++i)
	{
		auto card = this->deck.GetCard();
		if (card) this->hand.push_back(std::move(card));
	}
}

// Знищує всіх юнітів (без героїв) заданої сили в трьох рядах гравця —
// ефект карти "Спалення". Повертає межі знищених карт для анімації.
std::vector<sf::FloatRect> Player::CollectAndRemoveByPower(int power, const Board& board)
{
	std::vector<sf::FloatRect> destroyedBounds;

	auto removeFromRow = [&](std::vector<std::unique_ptr<Card>>& row, BoardRow rowType)
		{
			for (size_t i = 0; i < row.size(); )
			{
				if (row[i] &&
					row[i]->GetType() != CardType::Hero &&
					row[i]->GetCurPow(board, rowType) == power)
				{
					destroyedBounds.push_back(row[i]->GetBounds());
					this->AddToDiscard(std::move(row[i]));
					row.erase(row.begin() + static_cast<long long>(i));
				}
				else
				{
					++i;
				}
			}
		};

	removeFromRow(this->meleeRow, BoardRow::PlayerMelee);
	removeFromRow(this->rangedRow, BoardRow::PlayerRanged);
	removeFromRow(this->siegeRow, BoardRow::PlayerSiege);

	// Після видалення "дірок" перерозкладаємо решту карт без проміжків.
	auto repositionRow = [&](std::vector<std::unique_ptr<Card>>& row, float baseY)
		{
			for (size_t j = 0; j < row.size(); ++j)
			{
				if (row[j])
					row[j]->SetPosition(
						board.GetFieldX() + 20.f + static_cast<float>(j) * 165.f,
						baseY);
			}
		};

	float yMelee = board.GetPlayerStartY() + 10.f;
	float yRanged = board.GetPlayerStartY() + board.GetRowHeight() + board.GetRowGap() + 10.f;
	float ySiege = board.GetPlayerStartY() + 2.f * (board.GetRowHeight() + board.GetRowGap()) + 10.f;

	repositionRow(this->meleeRow, yMelee);
	repositionRow(this->rangedRow, yRanged);
	repositionRow(this->siegeRow, ySiege);

	return destroyedBounds;
}

// Розігрує виділену погодну карту без прив'язки до конкретного ряду
// (використовується, коли гравець явно тисне "покласти погоду").
bool Player::TryPlayWeather(Board& board)
{
	if (this->passed)
		return false;

	for (size_t i = 0; i < this->hand.size(); ++i)
	{
		if (!this->hand[i] || !this->hand[i]->IsSelected())
			continue;
		if (this->hand[i]->GetType() != CardType::Weather)
			return false;

		auto card = std::move(this->hand[i]);
		this->hand.erase(this->hand.begin() + static_cast<long long>(i));

		SpecialType st = card->GetSpecialType();
		BoardRow targetRow = BoardRow::PlayerMelee;
		if (st == SpecialType::Fog)  targetRow = BoardRow::PlayerRanged;
		if (st == SpecialType::Rain) targetRow = BoardRow::PlayerSiege;

		card->Play(board, targetRow);
		board.PlaceWeatherCard(std::move(card));
		this->TestHand();
		return true;
	}
	return false;
}

int Player::DeckSize() const
{
	return this->deck.GetSize();
}

void Player::AddToDiscard(std::unique_ptr<Card> c)
{
	if (c)
		this->discard.push_back(std::move(c));
}

const Card* Player::TopDiscard() const
{
	if (this->discard.empty())
		return nullptr;
	return this->discard.back().get();
}
