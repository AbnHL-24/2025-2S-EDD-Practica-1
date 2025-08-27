#pragma once
#include "../ds/queue.hpp"

class TurnManager {
    Queue<int> q; // player ids
    int current = -1;
    int roundIndex = 0;

public:
    void initCircular(int numPlayers) {
        for (int i = 0; i < numPlayers; ++i) q.enqueue(i);
        advance(); // set current
    }

    int getCurrent() const { return current; }
    int getRoundIndex() const { return roundIndex; }

    // Move to next player
    void advance() {
        if (q.isEmpty()) {
            current = -1;
            return;
        }
        int next = q.dequeue();
        // if current == -1 means first call
        if (current == -1) {
            current = next;
            q.enqueue(next);
            return;
        }
        current = next;
        q.enqueue(next);
        if (current == 0) ++roundIndex; // simplificado
    }

    void passTurn() {
        advance();
    }

    void bringToFront(int /*playerId*/) {
        // Pendiente: rotar cola para traer al frente un jugador específico
        // Por ahora, no implementado.
    }
};
