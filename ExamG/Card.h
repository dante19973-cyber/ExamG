#pragma once
#include <string>
class Card
{
	std::string name;
	int power;
public:
	Card();
	void Draw()const;
	int GetPower() const;
};