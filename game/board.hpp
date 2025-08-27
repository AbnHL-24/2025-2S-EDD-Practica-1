#pragma once
#include "../ds/linked_list.hpp"
#include "square.hpp"

struct Row { LinkedList<Square*> cells; };

class Board {
    LinkedList<Row*> rows; int R=0, C=0; // celdas
    // Mantén estructuras de edges H/V si te facilitan acceso por coordenadas
public:
    ~Board();
    void initializeFromPoints(int pointsW, int pointsH);
    enum class Dir { Up, Down, Left, Right };
    void expand(Dir d);
    // localizar edges por coordenada de puntos
    Edge* getEdgeH(int r, int c); // entre (r,c) y (r,c+1)
    Edge* getEdgeV(int r, int c); // entre (r,c) y (r+1,c)
    void collectClosedByEdge(Edge* e, LinkedList<Square*>& outSquares);
    void render(bool clairvoyant) const;
    void countRowWinnersPerPlayer(int numPlayers, int* outWinsPerPlayer) const;
    void countColWinnersPerPlayer(int numPlayers, int* outWinsPerPlayer) const;
    int maxIslandSizeForPlayer(int playerId) const;
};