#pragma once
#include<vector>
#include "Button.h"
class Menu
{
	std::vector<Button> buttons;
	bool isActive;
public:
	Menu();
	void Update();
	void Draw() const;
};

