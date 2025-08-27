#pragma once
#include "../ds/linked_list.hpp"
#include "square.hpp"

// Estructura que representa una fila de celdas en el tablero
struct Row {
    LinkedList<Square *> cells; // Lista enlazada de punteros a cuadrados en esta fila
};

// Clase que representa el tablero de juego completo
class Board {
    // Modelamos las celdas usando listas enlazadas; los bordes son compartidos por celdas adyacentes
    LinkedList<Row *> rows; // Filas de celdas del tablero

    // Mantenemos cuadriculas de bordes explicitas para un acceso rápido
    Edge **H = nullptr; // Bordes horizontales [hRows x hCols] aplanados en arreglo 1D
    Edge **V = nullptr; // Bordes verticales [vRows x vCols] aplanados en arreglo 1D

    int cellsR = 0, cellsC = 0; // Número de cuadrados (filas y columnas de celdas)
    int pointsR = 0, pointsC = 0; // Número de puntos en la cuadrícula (vértices)

public:
    ~Board(); // Destructor que libera memoria del tablero

    // Inicializa el tablero basado en el número de puntos (vértices) especificados
    void initializeFromPoints(int pointsW, int pointsH); // Pw (ancho), Ph (alto)

    // Métodos de acceso a las dimensiones del tablero
    int cellsRows() const { return cellsR; } // Retorna número de filas de celdas
    int cellsCols() const { return cellsC; } // Retorna número de columnas de celdas
    int pointsRows() const { return pointsR; } // Retorna número de filas de puntos
    int pointsCols() const { return pointsC; } // Retorna número de columnas de puntos

    // Accesores de bordes: las coordenadas están en la cuadrícula de puntos para H y V
    Edge *getEdgeH(int r, int c); // Bordes H tamaño: pointsR x (pointsC-1)
    Edge *getEdgeV(int r, int c); // Bordes V tamaño: (pointsR-1) x pointsC

    // Dado un borde, recolecta los cuadrados adyacentes que podrían cerrarse con él
    void collectSquaresAdjacentToEdge(Edge *e, LinkedList<Square *> &out);

    // Renderizado del tablero en consola
    void render(bool clairvoyant) const; // clairvoyant: muestra power-ups ocultos

    // Distribución de power-ups en el tablero (colocación aleatoria simple o determinística)
    void distributePowerUps(int percent /*0..100*/); // percent: porcentaje de cuadrados con power-ups

    // Métodos auxiliares para evaluación y acceso a elementos
    Row *getRow(int r) const; // Obtiene puntero a la fila especificada
    Square *getSquare(int r, int c) const; // Obtiene puntero al cuadrado en coordenadas específicas
};
