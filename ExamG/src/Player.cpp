#include "../include/Player.h"
#include<iostream>
Player::Player(const sf::Font& font)
	: deck(font)
{
	deck.Shuffle();
	for (int i = 0; i < 10;i++)
	{
		hand.push_back(deck.GetCard());
	}
	TestHand();
}
void Player::TestHand()
{
	float startX = 150.f;
	float y = 560.f;
	for (size_t i = 0; i < hand.size();i++)
	{
		hand[i]->SetPosition(startX + i * 95.f, y);
	}
}
void Player::AddCard(std::unique_ptr<Card> card)
{
	hand.push_back(std::move(card));
}
bool Player::PlayCard(BoardRow row, Board& board)
{
	if (passed)
		return false;
	for (size_t i = 0; i < hand.size(); ++i)
	{
		if (!hand[i]->IsSelected())
			continue;
		auto card = std::move(hand[i]);
			if (card->GetType() == CardType::Special)
			{
				card->Play(board);

				hand.erase(hand.begin() + i);
				TestHand();
				return true;
			}
			if (row==BoardRow::PlayerMelee)
			{
				meleeRow.push_back(std::move(card));
				meleeRow.back()->SetPosition(
					150.f + (meleeRow.size() - 1) * 95.f,
					490.f);
				meleeRow.back()->Play(board);
			}
			else if (row == BoardRow::PlayerRanged)
			{
				rangedRow.push_back(std::move(card));
				rangedRow.back()->SetPosition(
					150.f + (rangedRow.size() - 1) * 95.f,
					390.f);
				rangedRow.back()->Play(board);
			}
			hand.erase(hand.begin() + i);
			TestHand();
			return true;
	}
	return false;
}
void Player::Draw(sf::RenderWindow& window)
{
	static int counter = 0;
	if (counter++ % 60 == 0)
		std::cout << "Hand = " << hand.size() << std::endl;
	for (const auto& card : meleeRow)
		card->Draw(window);

	for (const auto& card : rangedRow)
		card->Draw(window);
	for(const auto& card : hand)
	{
		card->Draw(window);
	}
}
bool Player::HandleClick(sf::Vector2f mousePos)
{
	for (auto& card : hand)
	{
		if (card->Contains(mousePos))
		{
			for (auto& c : hand)
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
	for (const auto& card : meleeRow)
		sum += card->GetCurPow(board);
	return sum;
}
int Player::RangedP(const Board& board) const
{
	int sum = 0;
	for (const auto& card : rangedRow)
		sum += card->GetCurPow(board);
	return sum;
}
int Player::TotalP(const Board& board) const
{
	return MeleeP(board)+RangedP(board);
}
void Player::Pass() 
{
	passed = true;
}
bool Player::HasPassed() const
{
	return passed;
}
void Player::LiveLose()
{
	lives--;
}
int Player::GetLives()const
{
	return lives;
}
int Player::HandSize()const
{
	return hand.size();
}
void Player::CleanBoard()
{
	meleeRow.clear();
	rangedRow.clear();
	passed = false;
	for (auto& card : hand)
		card->SetSelected(false);
	TestHand();
}
void Player::DrawCard()
{
	if (!deck.IsEmpty())
		hand.push_back(deck.GetCard());
	TestHand();
}