#pragma once
#include <iostream>

// Інтерфейс (чистий абстрактний клас: без полів, лише чисто віртуальні
// методи). Його реалізує будь-який клас, який уміє зберігати себе у файл
// і відновлюватися з файлу.
class IPersistable
{
public:
	virtual ~IPersistable() = default;

	virtual void Save(std::ostream& out) const = 0; // записати стан у потік
	virtual void Load(std::istream& in) = 0;        // прочитати стан з потоку
};
