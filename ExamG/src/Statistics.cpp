#include "../include/Statistics.h"
#include <fstream>

// Конструктор: запам'ятовуємо шлях до файлу і одразу підвантажуємо
// збережений раніше рахунок (якщо файл існує).
Statistics::Statistics(const std::string& path)
	: filePath(path)
{
	this->LoadFromFile();
}

// Деструктор: перед знищенням об'єкта зберігаємо поточний рахунок у файл.
Statistics::~Statistics()
{
	this->SaveToFile();
}

void Statistics::RecordWin()
{
	this->counters.Increase(KEY_WINS);
}

void Statistics::RecordLoss()
{
	this->counters.Increase(KEY_LOSSES);
}

void Statistics::RecordDraw()
{
	this->counters.Increase(KEY_DRAWS);
}

int Statistics::GetWins() const
{
	return this->counters.Get(KEY_WINS);
}

int Statistics::GetLosses() const
{
	return this->counters.Get(KEY_LOSSES);
}

int Statistics::GetDraws() const
{
	return this->counters.Get(KEY_DRAWS);
}

int Statistics::GetGamesPlayed() const
{
	return this->GetWins() + this->GetLosses() + this->GetDraws();
}

// Записуємо кожен лічильник окремим рядком у форматі "ключ значення"
void Statistics::Save(std::ostream& out) const
{
	out << KEY_WINS << ' ' << this->GetWins() << '\n';
	out << KEY_LOSSES << ' ' << this->GetLosses() << '\n';
	out << KEY_DRAWS << ' ' << this->GetDraws() << '\n';
}

// Читаємо пари "ключ значення", поки вони є у потоці
void Statistics::Load(std::istream& in)
{
	std::string key;
	int value = 0;
	while (in >> key >> value)
	{
		this->counters.Set(key, value);
	}
}

void Statistics::SaveToFile() const
{
	std::ofstream file(this->filePath);
	if (file.is_open())
		this->Save(file);
}

void Statistics::LoadFromFile()
{
	std::ifstream file(this->filePath);
	if (file.is_open())
		this->Load(file);
}
