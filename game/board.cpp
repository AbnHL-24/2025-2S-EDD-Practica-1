#include "board.hpp"
#include <iostream>

static int idxH(int r, int c, int rows, int cols) { return r * cols + c; }
static int idxV(int r, int c, int rows, int cols) { return r * cols + c; }

Board::~Board() {
    // Free Squares and Rows
    for (int r = 0; r < cellsR; ++r) {
        Row *row = getRow(r);
        // delete Square*
        for (int c = 0; c < cellsC; ++c) {
            Square *s = row->cells.getAt(c);
            delete s;
        }
        delete row;
    }
    // Free edges
    if (H) {
        int hRows = pointsR;
        int hCols = pointsC - 1;
        for (int i = 0; i < hRows * hCols; ++i) delete H[i];
        delete[] H;
    }
    if (V) {
        int vRows = pointsR - 1;
        int vCols = pointsC;
        for (int i = 0; i < vRows * vCols; ++i) delete V[i];
        delete[] V;
    }
}

void Board::initializeFromPoints(int pointsW, int pointsH) {
    // points grid must be at least 3x3
    if (pointsW < 3) pointsW = 3;
    if (pointsH < 3) pointsH = 3;
    pointsC = pointsW;
    pointsR = pointsH;

    cellsC = pointsC - 1;
    cellsR = pointsR - 1;

    // Allocate edges grids
    int hRows = pointsR;
    int hCols = pointsC - 1;
    H = new Edge *[hRows * hCols];
    int vRows = pointsR - 1;
    int vCols = pointsC;
    V = new Edge *[vRows * vCols];

    int eid = 0;
    for (int r = 0; r < hRows; ++r) {
        for (int c = 0; c < hCols; ++c) {
            Edge *e = new Edge();
            e->id = eid++;
            e->o = Orient::H;
            e->r = r;
            e->c = c;
            H[idxH(r, c, hRows, hCols)] = e;
        }
    }
    for (int r = 0; r < vRows; ++r) {
        for (int c = 0; c < vCols; ++c) {
            Edge *e = new Edge();
            e->id = eid++;
            e->o = Orient::V;
            e->r = r;
            e->c = c;
            V[idxV(r, c, vRows, vCols)] = e;
        }
    }

    // Build cells as linked lists, wiring edges
    for (int r = 0; r < cellsR; ++r) {
        Row *row = new Row();
        for (int c = 0; c < cellsC; ++c) {
            Square *s = new Square();
            s->r = r;
            s->c = c;
            // Map edges: careful with coordinates
            // For square at (r,c):
            // top H at (r, c), bottom H at (r+1, c)
            // left V at (r, c), right V at (r, c+1)
            s->top = H[idxH(r, c, pointsR, pointsC - 1)];
            s->bottom = H[idxH(r + 1, c, pointsR, pointsC - 1)];
            s->left = V[idxV(r, c, pointsR - 1, pointsC)];
            s->right = V[idxV(r, c + 1, pointsR - 1, pointsC)];

            row->cells.push_back(s);
        }
        rows.push_back(row);
    }
}

Edge *Board::getEdgeH(int r, int c) {
    int hRows = pointsR, hCols = pointsC - 1;
    if (r < 0 || r >= hRows || c < 0 || c >= hCols) return nullptr;
    return H[idxH(r, c, hRows, hCols)];
}

Edge *Board::getEdgeV(int r, int c) {
    int vRows = pointsR - 1, vCols = pointsC;
    if (r < 0 || r >= vRows || c < 0 || c >= vCols) return nullptr;
    return V[idxV(r, c, vRows, vCols)];
}

Row *Board::getRow(int r) const {
    // rows is LinkedList<Row*>, getAt is O(n)
    return rows.getAt(r);
}

Square *Board::getSquare(int r, int c) const {
    return getRow(r)->cells.getAt(c);
}

void Board::collectSquaresAdjacentToEdge(Edge *e, LinkedList<Square *> &out) {
    if (!e) return;
    if (e->o == Orient::H) {
        int sr = e->r - 1;
        int sc = e->c;
        if (sr >= 0 && sr < cellsR && sc >= 0 && sc < cellsC) {
            out.push_back(getSquare(sr, sc));
        }
        sr = e->r;
        if (sr >= 0 && sr < cellsR && sc >= 0 && sc < cellsC) {
            out.push_back(getSquare(sr, sc));
        }
    } else {
        int sr = e->r;
        int sc = e->c - 1;
        if (sr >= 0 && sr < cellsR && sc >= 0 && sc < cellsC) {
            out.push_back(getSquare(sr, sc));
        }
        sc = e->c;
        if (sr >= 0 && sr < cellsR && sc >= 0 && sc < cellsC) {
            out.push_back(getSquare(sr, sc));
        }
    }
}

void Board::distributePowerUps(int percent) {
    if (percent <= 0) return;
    for (int r = 0; r < cellsR; ++r) {
        for (int c = 0; c < cellsC; ++c) {
            Square *s = getSquare(r, c);
            // Simple deterministic pseudo "random" to avoid RNG for now
            int hash = (r * 31 + c * 17 + 7) % 100;
            if (hash < percent) {
                // Alternate between some powers; puedes cambiar esta lógica
                int pick = (r + c) % 6;
                s->hasPower = true;
                switch (pick) {
                    case 0: s->power = {PowerType::DL};
                        break;
                    case 1: s->power = {PowerType::TS};
                        break;
                    case 2: s->power = {PowerType::BL};
                        break;
                    case 3: s->power = {PowerType::LS};
                        break;
                    case 4: s->power = {PowerType::ES};
                        break;
                    default: s->power = {PowerType::UF};
                        break;
                }
            }
        }
    }
}

void Board::render(bool clairvoyant) const {
    // Render uses fixed cell width=3
    const int w = 3;
    (void) w;
    // For each points row
    for (int pr = 0; pr < pointsR; ++pr) {
        // Draw horizontal edges row: points and H edges
        // Points row has pointsC points and (pointsC-1) H edges
        for (int pc = 0; pc < pointsC; ++pc) {
            std::cout << "*";
            if (pc < pointsC - 1) {
                Edge *e = H[idxH(pr, pc, pointsR, pointsC - 1)];
                if (e->placed) {
                    if (clairvoyant) {
                        // show codes inline up to width
                        std::cout << "_-_";
                    } else {
                        std::cout << "---";
                    }
                } else {
                    std::cout << "   ";
                }
            }
        }
        std::cout << "\n";

        // If not last points row, print verticals and cell interiors
        if (pr < pointsR - 1) {
            for (int pc = 0; pc < pointsC; ++pc) {
                // Vertical edge at (pr, pc)
                Edge *v = V[idxV(pr, pc, pointsR - 1, pointsC)];
                if (v->placed) std::cout << "l";
                else std::cout << " ";

                // Cell interior between (pc) and (pc+1) if exists
                if (pc < pointsC - 1) {
                    Square *s = getSquare(pr, pc);
                    char mid = ' ';
                    if (s->closed) {
                        if (s->ownerId >= 0) {
                            mid = '#'; // placeholder visual
                        }
                    } else if (s->hasPower) {
                        const char *code = PowerCode(s->power.type);
                        // fit 3 width: center 2-letter code
                        std::cout << code;
                        // fill remaining width 1
                        std::cout << " ";
                        continue;
                    }
                    // default fill
                    std::cout << " " << mid << " ";
                }
            }
            std::cout << "\n";
        }
    }

    if (clairvoyant) {
        // Second pass with effects annotations under the grid (simple list)
        std::cout << "\n[Clarividente] Efectos en lineas:\n";
        // List some sample effects found
        int hRows = pointsR, hCols = pointsC - 1;
        for (int r = 0; r < hRows; ++r) {
            for (int c = 0; c < hCols; ++c) {
                Edge *e = H[idxH(r, c, hRows, hCols)];
                bool any = false;
                for (auto it = e->effects.iter(); it.has(); it.next()) {
                    if (it.val().active) {
                        if (!any) {
                            std::cout << "H(" << r << "," << c << "):";
                            any = true;
                        }
                        std::cout << "[" << PowerCode(it.val().type) << "]";
                    }
                }
                if (any) std::cout << " ";
            }
        }
        int vRows = pointsR - 1, vCols = pointsC;
        for (int r = 0; r < vRows; ++r) {
            for (int c = 0; c < vCols; ++c) {
                Edge *e = V[idxV(r, c, vRows, vCols)];
                bool any = false;
                for (auto it = e->effects.iter(); it.has(); it.next()) {
                    if (it.val().active) {
                        if (!any) {
                            std::cout << "V(" << r << "," << c << "):";
                            any = true;
                        }
                        std::cout << "[" << PowerCode(it.val().type) << "]";
                    }
                }
                if (any) std::cout << " ";
            }
        }
        std::cout << "\n";
    }
}
