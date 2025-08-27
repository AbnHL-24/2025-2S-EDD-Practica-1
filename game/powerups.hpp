#pragma once

// Enumeración que define los diferentes tipos de power-ups disponibles en el juego
enum class PowerType {
    DL, // Double Laser - Láser doble
    TS, // Triple Shot - Disparo triple
    BL, // Big Laser - Láser grande
    PS, // Power Shot - Disparo potenciado
    LS, // Laser Shield - Escudo láser
    ES, // Energy Shield - Escudo de energía
    UF, // Ultra Fire - Fuego ultra
    AC, // Auto Cannon - Cañón automático
    NT, // Nuclear Torpedo - Torpedo nuclear
    EX, // Extra Life - Vida extra
    None // Sin power-up
};

// Estructura que representa un power-up en el juego
struct PowerUp {
    PowerType type; // Tipo de power-up que contiene
};

// Función que convierte un tipo de power-up a su código de texto correspondiente
inline const char *PowerCode(PowerType t) {
    switch (t) {
        case PowerType::DL: return "DL"; // Retorna código para Double Laser
        case PowerType::TS: return "TS"; // Retorna código para Triple Shot
        case PowerType::BL: return "BL"; // Retorna código para Big Laser
        case PowerType::PS: return "PS"; // Retorna código para Power Shot
        case PowerType::LS: return "LS"; // Retorna código para Laser Shield
        case PowerType::ES: return "ES"; // Retorna código para Energy Shield
        case PowerType::UF: return "UF"; // Retorna código para Ultra Fire
        case PowerType::AC: return "AC"; // Retorna código para Auto Cannon
        case PowerType::NT: return "NT"; // Retorna código para Nuclear Torpedo
        case PowerType::EX: return "EX"; // Retorna código para Extra Life
        default: return "  "; // Retorna espacios para tipos no válidos o None
    }
}
