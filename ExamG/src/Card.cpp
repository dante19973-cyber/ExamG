#include "../include/Card.h"
#include "../include/Board.h"
#include "../include/ResourceManager.h"
#include <algorithm>
#include <iostream>

// Базовий конструктор: готує форму карти й обидва текстові підписи
// (назва та сила). Візуальне оформлення (арт, іконки) додається окремо
// через SetVis(), бо не кожна карта одразу має доступ до ResourceManager.
Card::Card(const sf::Font& fontRef, const std::string& cardName, int cardPower)
	: font(&fontRef),
	  name(cardName),
	  power(cardPower),
	  shape(sf::Vector2f(CardW, CardH)),
	  nameText(fontRef, sf::String(), 14),
	  powerText(fontRef, sf::String(), 24),
	  position({ 0.f, 0.f }),
	  selected(false),
	  art(nullptr), powerB(nullptr), typeI(nullptr)
{
	shape.setFillColor(sf::Color(30, 30, 30));
	shape.setOutlineThickness(2.f);
	shape.setOutlineColor(sf::Color::Black);

	nameText.setFont(fontRef);
	nameText.setCharacterSize(16);
	nameText.setFillColor(sf::Color::White);
	nameText.setString(name);
	nameText.setOutlineColor(sf::Color::Black);
	nameText.setOutlineThickness(1.5f);

	powerText.setFont(fontRef);
	powerText.setString(std::to_string(power));
	powerText.setCharacterSize(22);
	powerText.setFillColor(sf::Color::Red);
}

// Переміщує карту в нову точку та перекладає всі "прикраси" (арт, іконки)
// відносно неї.
void Card::SetPosition(float x, float y)
{
	this->position = { x, y };
	this->shape.setPosition(this->position);
	this->LayVis();
}

void Card::Draw(sf::RenderWindow& window) const
{
	window.draw(this->shape);
	if (this->hasArt)  window.draw(*this->art);
	if (this->hasBage) window.draw(*this->powerB);
	if (this->hasIcon) window.draw(*this->typeI);
	window.draw(this->nameText);
	window.draw(this->powerText);
}

int Card::GetPower() const
{
	return this->power;
}

// Виділяє карту (підсвічує рамку жовтим і трохи піднімає над рядом).
void Card::SetSelected(bool value)
{
	this->selected = value;
	float offsetY = this->selected ? -25.f : 0.f;
	this->shape.setOutlineColor(this->selected ? sf::Color::Yellow : sf::Color::Black);
	this->shape.setPosition({ this->position.x, this->position.y + offsetY });
	this->LayVis();
}

bool Card::IsSelected() const
{
	return this->selected;
}

bool Card::Contains(sf::Vector2f mousePos) const
{
	return this->shape.getGlobalBounds().contains(mousePos);
}

std::string Card::GetName() const
{
	return this->name;
}

// Базова сила карти без урахування ефектів поля (погода/ріг тощо).
// Перевизначається в нащадках, яким потрібна інша логіка підрахунку.
int Card::GetCurPow(const Board&, BoardRow) const
{
	return this->power;
}

// Готує зовнішній вигляд карти: арт, значок сили та іконку типу.
// Кожен елемент завантажується окремо й безпечно ігнорується,
// якщо відповідної текстури немає (щоб відсутній файл не валив гру).
void Card::SetVis(ResourceManager& res, const std::string& artKey, CardType type, SpecialType st)
{
	this->shape.setSize({ CardW, CardH });
	this->shape.setFillColor(sf::Color(20, 20, 20));

	this->hasArt = false;
	this->hasBage = false;
	this->hasIcon = false;
	this->art.reset();
	this->powerB.reset();
	this->typeI.reset();

	if (!artKey.empty())
	{
		try
		{
			this->art = std::make_unique<sf::Sprite>(res.GetTexture(artKey));
			this->hasArt = true;
		}
		catch (const std::exception& ex)
		{
			std::cerr << "Art missing: " << ex.what() << std::endl;
		}
	}

	// Значок сили показуємо лише юнітам і героям.
	if (type == CardType::Unit || type == CardType::Hero)
	{
		try
		{
			const char* badgeKey = (type == CardType::Hero) ? "heroR" : "normR";
			this->powerB = std::make_unique<sf::Sprite>(res.GetTexture(badgeKey));
			this->hasBage = true;
		}
		catch (const std::exception& ex)
		{
			std::cerr << "Power badge missing: " << ex.what() << std::endl;
		}
	}

	// Іконку типу (погода/ріг/спалення/підміна) обираємо за SpecialType.
	if (type == CardType::Weather || type == CardType::Horn ||
		type == CardType::Scorch || type == CardType::Decoy)
	{
		std::string iconKey = "frostR";
		if (st == SpecialType::Fog)               iconKey = "fogR";
		else if (st == SpecialType::Rain)         iconKey = "rainR";
		else if (st == SpecialType::ClearWeather) iconKey = "sunnyR";
		else if (st == SpecialType::Horn)         iconKey = "hornR";
		else if (st == SpecialType::Scorch)       iconKey = "scorchR";
		else if (st == SpecialType::Decoy)        iconKey = "decoyR";

		try
		{
			this->typeI = std::make_unique<sf::Sprite>(res.GetTexture(iconKey));
			this->hasIcon = true;
		}
		catch (const std::exception& ex)
		{
			std::cerr << "Icon missing: " << ex.what() << std::endl;
		}
	}

	this->LayVis();
}

// Розкладає арт, значок сили, іконку типу й текст назви відносно
// поточної позиції та розміру карти. Викликається щоразу, коли карта
// рухається або міняє видимі елементи.
void Card::LayVis()
{
	const float x = this->shape.getPosition().x;
	const float y = this->shape.getPosition().y;
	const float w = this->shape.getSize().x;
	const float h = this->shape.getSize().y;

	if (this->hasArt)
	{
		float padL = w * 0.01f;
		float padT = h * 0.01f;
		float padR = w * 0.01f;
		float padB = h * 0.01f;

		sf::Vector2u ts = this->art->getTexture().getSize();
		float availW = w - padL - padR;
		float availH = h - padT - padB;
		float scale = std::min(availW / float(ts.x), availH / float(ts.y));

		this->art->setScale({ scale, scale });
		this->art->setPosition({
			x + padL + (availW - ts.x * scale) * 0.5f,
			y + padT + (availH - ts.y * scale) * 0.5f
			});
	}

	if (this->hasBage)
	{
		float badgeH = 90.f;
		sf::Vector2u ts = this->powerB->getTexture().getSize();
		float scale = badgeH / float(ts.y);
		this->powerB->setScale({ scale, scale });
		this->powerB->setPosition({ x - 6.f, y - 6.f });

		sf::FloatRect badgeBounds = this->powerB->getGlobalBounds();
		float centerX = badgeBounds.position.x + badgeBounds.size.x * 0.5f;
		float centerY = badgeBounds.position.y + badgeBounds.size.y * 0.5f;

		sf::FloatRect textBounds = this->powerText.getLocalBounds();
		this->powerText.setOrigin({
			textBounds.position.x + textBounds.size.x * 0.5f,
			textBounds.position.y + textBounds.size.y * 0.5f
			});
		this->powerText.setPosition({ centerX - 16.f, centerY - 16.f });
	}

	if (this->hasIcon)
	{
		float iconH = 45.f;
		sf::Vector2u ts = this->typeI->getTexture().getSize();
		float scale = iconH / float(ts.y);
		this->typeI->setScale({ scale, scale });
		this->typeI->setPosition({ x + w - iconH, y });
	}

	this->nameText.setPosition({ x + 8.f, y + h - 28.f });
}
