#include "../include/Bot.h"
Bot::Bot(const sf::Font& font)
    : deck(font)
{
    deck.Shuffle();

    for (int i = 0; i < 10; i++)
        hand.push_back(deck.GetCard());
}
void Bot::MakeMove(int playerPow, bool playerPass)
{
    if (passed)
        return;
    if (hand.empty())
    {
        Pass();
        return;
    }
    if (playerPass && TotalP() > playerPow)
    {
        Pass();
        return;
    }
    auto card = std::move(hand.front());
    if (rand() % 2 == 0)
    {
        meleeRow.push_back(std::move(card));
        meleeRow.back()->SetPosition(150.f + (meleeRow.size() - 1) * 95.f, 120.f);
    }
    else
    {
        rangedRow.push_back(std::move(card));
        rangedRow.back()->SetPosition(150.f + (rangedRow.size() - 1) * 95.f, 220.f);
    }
    hand.erase(hand.begin());
}
void Bot::TestHand()
{
    float startX = 150.f;
    float y = 20.f;
    for (size_t i = 0; i < hand.size();i++)
    {
        hand[i]->SetPosition(startX + i * 95.f, y);
    }
}
void Bot::Draw(sf::RenderWindow& window)
{
    for (const auto& card : meleeRow)
        card->Draw(window);

    for (const auto& card : rangedRow)
        card->Draw(window);
}
int Bot::MeleeP() const
{
    int sum = 0;
    for (const auto& card : meleeRow)
        sum += card->GetPower();
    return sum;
}
int Bot::RangedP() const
{
    int sum = 0;
    for (const auto& card : rangedRow)
        sum += card->GetPower();
    return sum;
}
int Bot::TotalP()const
{
    return MeleeP() + RangedP();
}
void Bot::Pass()
{
    passed = true;
}
bool Bot::HasPassed() const
{
    return passed;
}
void Bot::LiveLose()
{
    lives--;
}
int Bot::GetLives()const
{
    return lives;
}
int Bot::HandSize()const 
{
    return hand.size();
}
void Bot::CleanBoard()
{
    meleeRow.clear();
    rangedRow.clear();
    passed = false;
}
void Bot::DrawCard()
{
    if (!deck.IsEmpty())
        hand.push_back(deck.GetCard());
    TestHand();
}