#pragma once
#include <map>
#include <string>

// Власний шаблонний клас: тримає іменовані числові лічильники
// (наприклад "wins", "losses") у контейнері std::map.
// T - будь-який числовий тип (int, long і т.д.).
template <typename T>
class StatTracker
{
private:
	std::map<std::string, T> counters;

public:
	StatTracker() = default;
	~StatTracker() = default;

	// Збільшує лічильник з ім'ям key на amount (якщо його ще не було - створюється з 0)
	void Increase(const std::string& key, T amount = T(1))
	{
		this->counters[key] += amount;
	}

	// Задає лічильнику конкретне значення (використовується при читанні з файлу)
	void Set(const std::string& key, T value)
	{
		this->counters[key] = value;
	}

	// Повертає значення лічильника, або 0, якщо такого ще немає
	T Get(const std::string& key) const
	{
		auto it = this->counters.find(key);
		return (it != this->counters.end()) ? it->second : T();
	}
};
