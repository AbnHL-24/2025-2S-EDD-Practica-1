#pragma once
#include <string>
#include "../ds/stack.hpp"
#include "powerups.hpp"

// Estructura que representa a un jugador en el juego
struct Player {
    int id = 0; // Identificador único del jugador
    std::string name; // Nombre del jugador
    char initial = '?'; // Inicial del nombre del jugador para mostrar en interfaz

    int scorePoints = 0; // Puntos acumulados por el jugador
    int squaresOwned = 0; // Número de cuadrados completados por el jugador
    int powerUpsUsed = 0; // Contador de power-ups utilizados

    bool hasES = false; // Indica si tiene Energy Shield activo (Escudo de Energía)
    bool hasLS = false; // Indica si tiene Laser Shield activo (Escudo Láser)
    bool activeDL = false; // Indica si tiene Double Laser activo (Láser Doble)
    int dlRemaining = 0; // Número de usos restantes del Double Laser

    Stack<PowerUp> powerStack; // Pila de power-ups disponibles para usar
};
