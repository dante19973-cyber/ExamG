#include "../include/Bot.h"
#include "../include/ResourceManager.h"
#include "../include/Board.h"
#include <algorithm>

// Створює й тасує колоду бота, роздає стартову руку з 10 карт.
Bot::Bot(ResourceManager& res)
	: deck(res.GetFont("main"), res)
{
	this->deck.Shuffle();
	for (int i = 0; i < 10; ++i)
		this->hand.push_back(this->deck.GetCard());
}

// Проста, але передбачувана стратегія бота, у порядку пріоритету:
// 1) виставити найсильнішого юніта/героя в підходящий ряд;
// 2) зіграти ріг, якщо на полі вже є карти, які він підсилить;
// 3) спалити найсильніших юнітів гравця, якщо той виграє за очками;
// 4) зняти негоду ясною погодою або накласти нову негоду на гравця;
// 5) підмінити найсильнішу карту гравця, якщо той виграє за очками;
// 6) якщо гравець спасував — спасувати теж (нічого кращого зіграти);
// 7) скинути зайву підміну/ясну погоду;
// 8) інакше — спасувати.
void Bot::MakeMove(int playerPow, Board& board, bool playerPass)
{
	if (this->passed)
		return;
	if (this->hand.empty())
	{
		this->Pass();
		return;
	}
	// Якщо гравець уже спасував і зараз виграє за очками — більше
	// карт грати сенсу немає, теж пасуємо.
	if (playerPass && this->TotalP(board) > playerPow)
	{
		this->Pass();
		return;
	}

	const float rowHeight = board.GetRowHeight();
	const float rowGap = board.GetRowGap();
	const float enemyStartY = board.GetEnemyStartY();
	const float fieldX = board.GetFieldX();

	// Забирає карту з руки за індексом і повертає володіння нею.
	auto removeAt = [&](int idx) -> std::unique_ptr<Card>
		{
			auto card = std::move(this->hand[static_cast<size_t>(idx)]);
			this->hand.erase(this->hand.begin() + idx);
			return card;
		};

	// Один прохід по руці: знаходимо найсильнішого юніта/героя
	// та по одному представнику кожного типу спецкарти.
	int bestUnitPower = -1;
	int unitIdx = -1;
	int hornIdx = -1;
	int scorchIdx = -1;
	int frostIdx = -1, fogIdx = -1, rainIdx = -1, clearIdx = -1;

	for (size_t i = 0; i < this->hand.size(); ++i)
	{
		if (!this->hand[i])
			continue;

		CardType type = this->hand[i]->GetType();
		SpecialType special = this->hand[i]->GetSpecialType();

		if (type == CardType::Unit || type == CardType::Hero)
		{
			int power = this->hand[i]->GetPower();
			if (power > bestUnitPower)
			{
				bestUnitPower = power;
				unitIdx = static_cast<int>(i);
			}
		}
		else if (type == CardType::Horn && hornIdx < 0)
			hornIdx = static_cast<int>(i);
		else if (type == CardType::Scorch && scorchIdx < 0)
			scorchIdx = static_cast<int>(i);
		else if (type == CardType::Weather)
		{
			if (special == SpecialType::Frost && frostIdx < 0) frostIdx = static_cast<int>(i);
			else if (special == SpecialType::Fog && fogIdx < 0) fogIdx = static_cast<int>(i);
			else if (special == SpecialType::Rain && rainIdx < 0) rainIdx = static_cast<int>(i);
			else if (special == SpecialType::ClearWeather && clearIdx < 0) clearIdx = static_cast<int>(i);
		}
	}

	// 1) Виставляємо найсильнішого юніта/героя: слабких — в облогу
	// (щоб не постраждали від дощу першими), середніх — у дальній бій,
	// сильних — у ближній бій.
	if (unitIdx >= 0)
	{
		auto card = removeAt(unitIdx);
		int power = card->GetPower();

		BoardRow row = BoardRow::EnemyMelee;
		std::vector<std::unique_ptr<Card>>* targetRow = &this->meleeRow;
		float y = enemyStartY + 2.f * (rowHeight + rowGap) + 10.f;

		if (power <= 5)
		{
			row = BoardRow::EnemySiege;
			targetRow = &this->siegeRow;
			y = enemyStartY + 10.f;
		}
		else if (power <= 8)
		{
			row = BoardRow::EnemyRanged;
			targetRow = &this->rangedRow;
			y = enemyStartY + rowHeight + rowGap + 10.f;
		}

		targetRow->push_back(std::move(card));
		targetRow->back()->SetPosition(fieldX + 20.f + (targetRow->size() - 1) * 165.f, y);
		targetRow->back()->Play(board, row);
		return;
	}

	// 2) Ріг має сенс лише якщо на полі вже є карти, силу яких подвоїти.
	if (hornIdx >= 0 && this->TotalP(board) > 0)
	{
		int melee = this->MeleeP(board);
		int ranged = this->RangedP(board);
		int siege = this->SiegeP(board);

		BoardRow bestRow = BoardRow::EnemyMelee;
		if (ranged >= melee && ranged >= siege) bestRow = BoardRow::EnemyRanged;
		else if (siege >= melee && siege >= ranged) bestRow = BoardRow::EnemySiege;

		auto card = removeAt(hornIdx);
		card->Play(board, bestRow);
		board.SpCardPosition(bestRow, std::move(card));
		return;
	}

	// 3) Спалення грається лише коли бот програє за очками.
	if (scorchIdx >= 0 && this->TotalP(board) < playerPow)
	{
		auto card = removeAt(scorchIdx);
		card->Play(board, BoardRow::EnemyMelee);
		return;
	}

	// 4) Ясна погода знімає активну негоду; інакше накладаємо нову негоду.
	if (clearIdx >= 0 && (board.IsWeatherM() || board.IsWeatherR() || board.IsWeatherS()))
	{
		auto card = removeAt(clearIdx);
		card->Play(board, BoardRow::EnemyMelee);
		board.PlaceWeatherCard(std::move(card));
		return;
	}
	if (frostIdx >= 0 && !board.IsWeatherM())
	{
		auto card = removeAt(frostIdx);
		card->Play(board, BoardRow::EnemyMelee);
		board.PlaceWeatherCard(std::move(card));
		return;
	}
	if (fogIdx >= 0 && !board.IsWeatherR())
	{
		auto card = removeAt(fogIdx);
		card->Play(board, BoardRow::EnemyRanged);
		board.PlaceWeatherCard(std::move(card));
		return;
	}
	if (rainIdx >= 0 && !board.IsWeatherS())
	{
		auto card = removeAt(rainIdx);
		card->Play(board, BoardRow::EnemySiege);
		board.PlaceWeatherCard(std::move(card));
		return;
	}

	// 5) Якщо в руці є "Підміна" і бот програє за очками — забираємо
	// найсильнішу карту гравця з поля (герої не чіпаються).
	int decoyIdx = -1;
	for (size_t i = 0; i < this->hand.size(); ++i)
	{
		if (this->hand[i] && this->hand[i]->GetType() == CardType::Decoy)
		{
			decoyIdx = static_cast<int>(i);
			break;
		}
	}

	if (decoyIdx >= 0 && this->TotalP(board) < playerPow)
	{
		// Шукає найсильнішу не-героя карту в одному ряду поля бота
		// й оновлює найкращий знайдений варіант, якщо він сильніший.
		struct BestTarget
		{
			std::vector<std::unique_ptr<Card>>* row = nullptr;
			size_t index = 0;
			int power = 0;
			float y = 0.f;
		} best;

		auto scanRow = [&](std::vector<std::unique_ptr<Card>>& row, BoardRow rowType, float baseY)
			{
				for (size_t i = 0; i < row.size(); ++i)
				{
					if (!row[i] || row[i]->GetType() == CardType::Hero)
						continue;

					int power = row[i]->GetCurPow(board, rowType);
					if (power > best.power)
					{
						best.power = power;
						best.index = i;
						best.row = &row;
						best.y = baseY;
					}
				}
			};

		scanRow(this->meleeRow, BoardRow::EnemyMelee, enemyStartY + 10.f);
		scanRow(this->rangedRow, BoardRow::EnemyRanged, enemyStartY + rowHeight + rowGap + 10.f);
		scanRow(this->siegeRow, BoardRow::EnemySiege, enemyStartY + 2.f * (rowHeight + rowGap) + 10.f);

		if (best.row && best.power > 0)
		{
			std::unique_ptr<Card> capturedCard = std::move((*best.row)[best.index]);
			best.row->erase(best.row->begin() + static_cast<long long>(best.index));

			auto decoy = removeAt(decoyIdx);
			decoy->SetSelected(false);
			best.row->push_back(std::move(decoy));

			for (size_t j = 0; j < best.row->size(); ++j)
				if ((*best.row)[j])
					(*best.row)[j]->SetPosition(fieldX + 20.f + j * 165.f, best.y);

			capturedCard->SetSelected(false);
			this->hand.push_back(std::move(capturedCard));
			return;
		}
	}

	// 6) Гравець уже спасував, а в нас немає вигідного ходу — пасуємо теж.
	if (playerPass)
	{
		this->Pass();
		return;
	}

	// 7) Немає сенсу тримати непотрібну підміну чи зайву ясну погоду —
	// тихо скидаємо одну з них, щоб звільнити місце в руці.
	for (size_t i = 0; i < this->hand.size(); ++i)
	{
		if (!this->hand[i]) continue;
		CardType type = this->hand[i]->GetType();
		if (type == CardType::Decoy ||
			(type == CardType::Weather && this->hand[i]->GetSpecialType() == SpecialType::ClearWeather))
		{
			this->hand.erase(this->hand.begin() + static_cast<long long>(i));
			return;
		}
	}

	// 8) Жодного корисного ходу не знайшлося.
	this->Pass();
}

// Розкладає карти руки бота в компактний ряд у верхній частині екрана.
void Bot::TestHand()
{
	const float startX = 270.f;
	const float y = 5.f;
	for (size_t i = 0; i < this->hand.size(); ++i)
	{
		this->hand[i]->SetPosition(startX + i * 95.f, y);
	}
}

void Bot::Draw(sf::RenderWindow& window)
{
	for (const auto& card : this->meleeRow)
		card->Draw(window);
	for (const auto& card : this->rangedRow)
		card->Draw(window);
	for (const auto& card : this->siegeRow)
		card->Draw(window);
}

int Bot::MeleeP(const Board& board) const
{
	int sum = 0;
	for (const auto& card : this->meleeRow)
		sum += card->GetCurPow(board, BoardRow::EnemyMelee);
	return sum;
}

int Bot::RangedP(const Board& board) const
{
	int sum = 0;
	for (const auto& card : this->rangedRow)
		sum += card->GetCurPow(board, BoardRow::EnemyRanged);
	return sum;
}

int Bot::SiegeP(const Board& board) const
{
	int sum = 0;
	for (const auto& card : this->siegeRow)
		sum += card->GetCurPow(board, BoardRow::EnemySiege);
	return sum;
}

int Bot::TotalP(const Board& board) const
{
	return this->MeleeP(board) + this->RangedP(board) + this->SiegeP(board);
}

void Bot::Pass()
{
	this->passed = true;
}

bool Bot::HasPassed() const
{
	return this->passed;
}

void Bot::LiveLose()
{
	if (this->lives > 0)
		this->lives--;
}

int Bot::GetLives() const
{
	return this->lives;
}

int Bot::HandSize() const
{
	return static_cast<int>(this->hand.size());
}

// Переносить усі карти бота з поля в скидання перед новим раундом.
void Bot::CleanBoard()
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
}

void Bot::DrawCard()
{
	if (!this->deck.IsEmpty())
		this->hand.push_back(this->deck.GetCard());
	this->TestHand();
}

// Найбільша сила серед юнітів бота (без героїв) по всіх трьох рядах.
int Bot::MaxUnitPower(const Board& board) const
{
	int maxPow = 0;
	auto checkRow = [&](const std::vector<std::unique_ptr<Card>>& row, BoardRow rowType)
		{
			for (const auto& card : row)
			{
				if (card && card->GetType() != CardType::Hero)
					maxPow = std::max(maxPow, card->GetCurPow(board, rowType));
			}
		};

	checkRow(this->meleeRow, BoardRow::EnemyMelee);
	checkRow(this->rangedRow, BoardRow::EnemyRanged);
	checkRow(this->siegeRow, BoardRow::EnemySiege);
	return maxPow;
}

// Повністю скидає бота до стану нової партії.
void Bot::FullReset(ResourceManager& res)
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

// Знищує всіх юнітів (без героїв) заданої сили в рядах бота —
// ефект карти "Спалення", розіграної гравцем.
std::vector<sf::FloatRect> Bot::CollectAndRemoveByPower(int power, const Board& board)
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

	removeFromRow(this->meleeRow, BoardRow::EnemyMelee);
	removeFromRow(this->rangedRow, BoardRow::EnemyRanged);
	removeFromRow(this->siegeRow, BoardRow::EnemySiege);

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

	float ySiege = board.GetEnemyStartY() + 10.f;
	float yRanged = board.GetEnemyStartY() + board.GetRowHeight() + board.GetRowGap() + 10.f;
	float yMelee = board.GetEnemyStartY() + 2.f * (board.GetRowHeight() + board.GetRowGap()) + 10.f;

	repositionRow(this->siegeRow, ySiege);
	repositionRow(this->rangedRow, yRanged);
	repositionRow(this->meleeRow, yMelee);

	return destroyedBounds;
}

int Bot::DeckSize() const
{
	return this->deck.GetSize();
}

void Bot::AddToDiscard(std::unique_ptr<Card> c)
{
	if (c)
		this->discard.push_back(std::move(c));
}

const Card* Bot::TopDiscard() const
{
	if (this->discard.empty())
		return nullptr;
	return this->discard.back().get();
}
