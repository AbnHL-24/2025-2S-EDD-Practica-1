#pragma once
#include <string>
#include "../ds/stack.hpp"
#include "powerups.hpp"

struct Player {
    int id = 0;
    std::string name;
    char initial = '?';

    int scorePoints = 0;
    int squaresOwned = 0;
    int powerUpsUsed = 0;

    bool hasES = false; // Escurridizo buffer
    bool hasLS = false; // Llave Secreta buffer
    bool activeDL = false;
    int dlRemaining = 0; // Doble Línea en curso

    Stack<PowerUp> powerStack;
};
