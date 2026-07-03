#pragma once
#include<string>
#include"Card.h"
class SpecialCard : public Card
{
	std::string effect;
	int bonus;
public:
	SpecialCard();
	void Activate();
	void Draw()const;
};

