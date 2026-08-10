#pragma once
#include <string>
#include "IPersistable.h"
#include "StatTracker.h"

// Клас Statistics зберігає кількість перемог/поразок/нічиїх гравця
// і автоматично зберігає/відновлює цей рахунок у текстовому файлі
// між запусками програми.
//
// - реалізує інтерфейс IPersistable
// - для самих лічильників використовує власний шаблонний клас StatTracker<int>
class Statistics : public IPersistable
{
	StatTracker<int> counters; // лічильники "wins"/"losses"/"draws"
	std::string filePath;      // шлях до файлу зі статистикою

	static constexpr const char* KEY_WINS = "wins";
	static constexpr const char* KEY_LOSSES = "losses";
	static constexpr const char* KEY_DRAWS = "draws";

public:
	explicit Statistics(const std::string& path = "gwent_stats.txt");
	~Statistics() override; // при знищенні об'єкта статистика зберігається у файл

	// Методи-мутатори: викликаються по завершенню матчу
	void RecordWin();
	void RecordLoss();
	void RecordDraw();

	// Методи-аксесори
	int GetWins() const;
	int GetLosses() const;
	int GetDraws() const;
	int GetGamesPlayed() const;

	// Реалізація інтерфейсу IPersistable
	void Save(std::ostream& out) const override;
	void Load(std::istream& in) override;

private:
	void SaveToFile() const; // відкриває файл і викликає Save()
	void LoadFromFile();     // відкриває файл і викликає Load()
};
