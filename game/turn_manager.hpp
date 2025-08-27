#pragma once
#include "../ds/queue.hpp"

// Clase que gestiona el sistema de turnos circulares entre jugadores
class TurnManager {
    Queue<int> q; // Cola circular de IDs de jugadores para gestionar turnos
    int current = -1; // ID del jugador con el turno actual (-1 si no hay turno activo)
    int roundIndex = 0; // Índice de la ronda actual del juego

public:
    // Inicializa el sistema de turnos con el número especificado de jugadores
    void initCircular(int numPlayers) {
        for (int i = 0; i < numPlayers; ++i) q.enqueue(i); // Agrega todos los IDs a la cola
        advance(); // Establece el primer jugador actual
    }

    int getCurrent() const { return current; } // Retorna el ID del jugador con turno actual
    int getRoundIndex() const { return roundIndex; } // Retorna el índice de la ronda actual

    // Avanza al siguiente jugador en el orden circular de turnos
    void advance() {
        if (q.isEmpty()) {
            // Si no hay jugadores en cola
            current = -1; // No hay turno activo
            return;
        }
        int next = q.dequeue(); // Extrae el siguiente jugador de la cola
        // Si current == -1 significa la primera llamada (inicialización)
        if (current == -1) {
            current = next; // Establece el primer jugador actual
            q.enqueue(next); // Lo reinserta al final de la cola
            return;
        }
        current = next; // Actualiza el jugador actual
        q.enqueue(next); // Reinserta al final para mantener circularidad
        if (current == 0) ++roundIndex; // Incrementa ronda cuando vuelve al jugador 0 (simplificado)
    }

    // Pasa el turno al siguiente jugador (alias de advance para claridad semántica)
    void passTurn() {
        advance();
    }

    // Trae un jugador específico al frente de la cola de turnos
    void bringToFront(int /*playerId*/) {
        // Pendiente: rotar cola para traer al frente un jugador específico
        // Por ahora, no implementado.
    }
};
