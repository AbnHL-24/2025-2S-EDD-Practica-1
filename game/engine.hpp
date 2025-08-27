#pragma once
#include "../ds/linked_list.hpp"
#include "board.hpp"
#include "player.hpp"
#include "turn_manager.hpp"
#include "powerups.hpp"

// Clase principal que controla la lógica del juego y coordina todos los componentes
class Engine {
    Board board; // Tablero de juego que contiene la cuadrícula, bordes y cuadrados
    LinkedList<Player *> players; // Lista de jugadores participando en la partida
    TurnManager tm; // Gestor de turnos circulares entre jugadores

    bool clairvoyant = false; // Modo de vista clarividente (muestra power-ups ocultos)

public:
    ~Engine(); // Destructor que libera memoria de jugadores

    // Configuración inicial del juego mediante interacción con el usuario
    void setupInteractively();

    // Bucle principal del juego que maneja el menú y las acciones
    void gameLoop();

private:
    // Obtiene un jugador específico por su ID
    Player *getPlayer(int id) const;

    // Renderiza el tablero en consola con información de jugadores y turno actual
    void renderBoard();

    // Muestra los puntajes de todos los jugadores
    void printScores();

    // === Acciones del jugador ===

    // Acción para colocar una línea en el tablero
    void actionPlaceLine();

    // Acción para usar un power-up de la pila del jugador
    void actionUsePower();

    // === Reglas internas del juego ===

    // Hook llamado cuando se coloca una línea (para logging o efectos adicionales)
    void onLinePlaced(Edge *e, Player *p);

    // Resuelve el cierre de cuadrados cuando se coloca una línea
    void resolveSquaresForEdge(Edge *e, Player *p);

    // === Funciones auxiliares ===

    // Añade un power-up a la pila de un jugador
    void pushPower(Player *p, PowerType t);

    // Verifica si el juego ha terminado (todos los cuadrados cerrados)
    bool isGameFinished() const;
};
