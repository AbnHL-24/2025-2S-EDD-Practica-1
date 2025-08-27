#pragma once
#include "../ds/linked_list.hpp"
#include "square.hpp"

struct Row {
    LinkedList<Square *> cells;
};

class Board {
    // We model cells using linked lists; edges are shared by adjacent cells
    LinkedList<Row *> rows; // rows of cells
    // Keep explicit edge grids for quick access
    Edge **H = nullptr; // horizontal edges [hRows x hCols] flattened
    Edge **V = nullptr; // vertical edges [vRows x vCols] flattened

    int cellsR = 0, cellsC = 0; // number of squares
    int pointsR = 0, pointsC = 0; // number of points grid

public:
    ~Board();

    void initializeFromPoints(int pointsW, int pointsH); // Pw, Ph
    int cellsRows() const { return cellsR; }
    int cellsCols() const { return cellsC; }
    int pointsRows() const { return pointsR; }
    int pointsCols() const { return pointsC; }

    // Edge accessors: coordinates are in points-grid for H and V
    Edge *getEdgeH(int r, int c); // H size: pointsR x (pointsC-1)
    Edge *getEdgeV(int r, int c); // V size: (pointsR-1) x pointsC

    // Given an edge, collect adjacent squares that could be closed by it
    void collectSquaresAdjacentToEdge(Edge *e, LinkedList<Square *> &out);

    // Rendering
    void render(bool clairvoyant) const;

    // Power-up distribution (simple random placement or deterministic)
    void distributePowerUps(int percent /*0..100*/);

    // Helpers for evaluation
    Row *getRow(int r) const;

    Square *getSquare(int r, int c) const;
};
