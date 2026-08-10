#pragma once

// Ряд ігрового поля, у який можна викласти карту.
// None означає "поза полем" (наприклад, карта ще в руці).
enum class BoardRow
{
    None,
    PlayerMelee,   // ближній бій гравця
    PlayerRanged,  // дальній бій гравця
    PlayerSiege,   // облога гравця
    EnemyMelee,    // ближній бій бота
    EnemyRanged,   // дальній бій бота
    EnemySiege     // облога бота
};
