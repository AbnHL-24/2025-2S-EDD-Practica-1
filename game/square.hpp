#pragma once
#include "edge.hpp"
#include "powerups.hpp"

// Estructura que representa un cuadrado/celda en la cuadrícula del juego
struct Square {
    int r = 0, c = 0; // Coordenadas de la celda en la cuadrícula (fila, columna)

    Edge *top = nullptr; // Puntero al borde superior del cuadrado
    Edge *right = nullptr; // Puntero al borde derecho del cuadrado
    Edge *bottom = nullptr; // Puntero al borde inferior del cuadrado
    Edge *left = nullptr; // Puntero al borde izquierdo del cuadrado

    bool closed = false; // Indica si el cuadrado está completamente cerrado (todos los bordes colocados)
    int ownerId = -1; // ID del propietario visual inicial del cuadrado
    int pointsAwardedTo = -1; // ID del jugador que recibió los puntos por cerrar este cuadrado

    bool hasPower = false; // Indica si el cuadrado contiene un power-up
    PowerUp power{PowerType::None}; // Power-up contenido en el cuadrado (None si no tiene)

    // Verifica si el cuadrado puede ser cerrado (todos los bordes están asignados y colocados)
    bool isClosable() const {
        return top && right && bottom && left && // Todos los bordes están asignados
               top->placed && right->placed && bottom->placed && left->placed; // Todos los bordes están colocados
    }
};
