#include "board.hpp"
#include <iostream>

// Funciones auxiliares para calcular índices en arreglos 1D que representan grillas 2D
static int idxH(int r, int c, int rows, int cols) { return r * cols + c; } // Índice para bordes horizontales
static int idxV(int r, int c, int rows, int cols) { return r * cols + c; } // Índice para bordes verticales

// Destructor del tablero: libera toda la memoria asignada dinámicamente
Board::~Board() {
    // Libera cuadrados y filas
    for (int r = 0; r < cellsR; ++r) {
        Row *row = getRow(r);
        // Elimina cada cuadrado en la fila
        for (int c = 0; c < cellsC; ++c) {
            Square *s = row->cells.getAt(c);
            delete s;
        }
        delete row; // Elimina la fila
    }
    // Libera bordes horizontales
    if (H) {
        int hRows = pointsR;
        int hCols = pointsC - 1;
        for (int i = 0; i < hRows * hCols; ++i) delete H[i]; // Elimina cada borde
        delete[] H; // Elimina el arreglo de punteros
    }
    // Libera bordes verticales
    if (V) {
        int vRows = pointsR - 1;
        int vCols = pointsC;
        for (int i = 0; i < vRows * vCols; ++i) delete V[i]; // Elimina cada borde
        delete[] V; // Elimina el arreglo de punteros
    }
}

// Inicializa el tablero basándose en el número de puntos (vértices) especificados
void Board::initializeFromPoints(int pointsW, int pointsH) {
    // La cuadrícula de puntos debe ser al menos 3x3
    if (pointsW < 3) pointsW = 3;
    if (pointsH < 3) pointsH = 3;
    pointsC = pointsW; // Columnas de puntos
    pointsR = pointsH; // Filas de puntos

    cellsC = pointsC - 1; // Columnas de celdas (una menos que puntos)
    cellsR = pointsR - 1; // Filas de celdas (una menos que puntos)

    // Asigna memoria para las cuadrículas de bordes
    int hRows = pointsR; // Bordes horizontales: mismas filas que puntos
    int hCols = pointsC - 1; // Bordes horizontales: una columna menos que puntos
    H = new Edge *[hRows * hCols];
    int vRows = pointsR - 1; // Bordes verticales: una fila menos que puntos
    int vCols = pointsC; // Bordes verticales: mismas columnas que puntos
    V = new Edge *[vRows * vCols];

    // Crea y configura bordes horizontales
    int eid = 0; // ID único para cada borde
    for (int r = 0; r < hRows; ++r) {
        for (int c = 0; c < hCols; ++c) {
            Edge *e = new Edge();
            e->id = eid++;
            e->o = Orient::H; // Orientación horizontal
            e->r = r;
            e->c = c;
            H[idxH(r, c, hRows, hCols)] = e;
        }
    }
    // Crea y configura bordes verticales
    for (int r = 0; r < vRows; ++r) {
        for (int c = 0; c < vCols; ++c) {
            Edge *e = new Edge();
            e->id = eid++;
            e->o = Orient::V; // Orientación vertical
            e->r = r;
            e->c = c;
            V[idxV(r, c, vRows, vCols)] = e;
        }
    }

    // Construye celdas como listas enlazadas, conectando bordes
    for (int r = 0; r < cellsR; ++r) {
        Row *row = new Row();
        for (int c = 0; c < cellsC; ++c) {
            Square *s = new Square();
            s->r = r;
            s->c = c;
            // Mapea bordes: cuidado con las coordenadas
            // Para cuadrado en (r,c):
            // borde superior H en (r, c), borde inferior H en (r+1, c)
            // borde izquierdo V en (r, c), borde derecho V en (r, c+1)
            s->top = H[idxH(r, c, pointsR, pointsC - 1)];
            s->bottom = H[idxH(r + 1, c, pointsR, pointsC - 1)];
            s->left = V[idxV(r, c, pointsR - 1, pointsC)];
            s->right = V[idxV(r, c + 1, pointsR - 1, pointsC)];

            row->cells.push_back(s);
        }
        rows.push_back(row);
    }
}

// Obtiene un borde horizontal en las coordenadas especificadas
Edge *Board::getEdgeH(int r, int c) {
    int hRows = pointsR, hCols = pointsC - 1;
    if (r < 0 || r >= hRows || c < 0 || c >= hCols) return nullptr; // Valida límites
    return H[idxH(r, c, hRows, hCols)];
}

// Obtiene un borde vertical en las coordenadas especificadas
Edge *Board::getEdgeV(int r, int c) {
    int vRows = pointsR - 1, vCols = pointsC;
    if (r < 0 || r >= vRows || c < 0 || c >= vCols) return nullptr; // Valida límites
    return V[idxV(r, c, vRows, vCols)];
}

// Obtiene un puntero a la fila especificada
Row *Board::getRow(int r) const {
    // rows es LinkedList<Row*>, getAt es O(n)
    return rows.getAt(r);
}

// Obtiene un puntero al cuadrado en las coordenadas especificadas
Square *Board::getSquare(int r, int c) const {
    return getRow(r)->cells.getAt(c);
}

// Recolecta los cuadrados adyacentes a un borde dado que podrían cerrarse con él
void Board::collectSquaresAdjacentToEdge(Edge *e, LinkedList<Square *> &out) {
    if (!e) return;
    if (e->o == Orient::H) {
        // Para bordes horizontales
        int sr = e->r - 1; // Cuadrado arriba del borde
        int sc = e->c;
        if (sr >= 0 && sr < cellsR && sc >= 0 && sc < cellsC) {
            out.push_back(getSquare(sr, sc));
        }
        sr = e->r; // Cuadrado abajo del borde
        if (sr >= 0 && sr < cellsR && sc >= 0 && sc < cellsC) {
            out.push_back(getSquare(sr, sc));
        }
    } else {
        // Para bordes verticales
        int sr = e->r;
        int sc = e->c - 1; // Cuadrado a la izquierda del borde
        if (sr >= 0 && sr < cellsR && sc >= 0 && sc < cellsC) {
            out.push_back(getSquare(sr, sc));
        }
        sc = e->c; // Cuadrado a la derecha del borde
        if (sr >= 0 && sr < cellsR && sc >= 0 && sc < cellsC) {
            out.push_back(getSquare(sr, sc));
        }
    }
}

// Distribuye power-ups aleatoriamente en el tablero según el porcentaje especificado
void Board::distributePowerUps(int percent) {
    if (percent <= 0) return;
    for (int r = 0; r < cellsR; ++r) {
        for (int c = 0; c < cellsC; ++c) {
            Square *s = getSquare(r, c);
            // Pseudo-aleatorio determinístico simple para evitar RNG por ahora
            int hash = (r * 31 + c * 17 + 7) % 100;
            if (hash < percent) {
                // Alterna entre algunos power-ups; puedes cambiar esta lógica
                int pick = (r + c) % 6;
                s->hasPower = true;
                switch (pick) {
                    case 0: s->power = {PowerType::DL};
                        break; // Double Laser
                    case 1: s->power = {PowerType::TS};
                        break; // Triple Shot
                    case 2: s->power = {PowerType::BL};
                        break; // Big Laser
                    case 3: s->power = {PowerType::LS};
                        break; // Laser Shield
                    case 4: s->power = {PowerType::ES};
                        break; // Energy Shield
                    default: s->power = {PowerType::UF};
                        break; // Ultra Fire
                }
            }
        }
    }
}

// Renderiza el tablero en la consola con representación ASCII
void Board::render(bool clairvoyant) const {
    // El renderizado usa ancho fijo de celda = 3
    const int w = 3;
    (void) w; // Suprime warning de variable no usada

    // Para cada fila de puntos
    for (int pr = 0; pr < pointsR; ++pr) {
        // Dibuja fila de bordes horizontales: puntos y bordes H
        // Fila de puntos tiene pointsC puntos y (pointsC-1) bordes H
        for (int pc = 0; pc < pointsC; ++pc) {
            std::cout << "*"; // Dibuja punto/vértice
            if (pc < pointsC - 1) {
                Edge *e = H[idxH(pr, pc, pointsR, pointsC - 1)];
                if (e->placed) {
                    // Si el borde está colocado
                    if (clairvoyant) {
                        // Muestra códigos inline hasta el ancho disponible
                        std::cout << "_-_";
                    } else {
                        std::cout << "---"; // Línea sólida
                    }
                } else {
                    std::cout << "   "; // Espacio vacío
                }
            }
        }
        std::cout << "\n";

        // Si no es la última fila de puntos, imprime verticales e interiores de celdas
        if (pr < pointsR - 1) {
            for (int pc = 0; pc < pointsC; ++pc) {
                // Borde vertical en (pr, pc)
                Edge *v = V[idxV(pr, pc, pointsR - 1, pointsC)];
                if (v->placed) std::cout << "l"; // Línea vertical
                else std::cout << " "; // Espacio vacío

                // Interior de celda entre (pc) y (pc+1) si existe
                if (pc < pointsC - 1) {
                    Square *s = getSquare(pr, pc);
                    char mid = ' ';
                    if (s->closed) {
                        // Si el cuadrado está cerrado
                        if (s->ownerId >= 0) {
                            mid = '#'; // Marcador visual del propietario
                        }
                    } else if (s->hasPower) {
                        // Si tiene power-up
                        const char *code = PowerCode(s->power.type);
                        // Ajusta a ancho 3: centra código de 2 letras
                        std::cout << code;
                        // Rellena ancho restante 1
                        std::cout << " ";
                        continue;
                    }
                    // Relleno por defecto
                    std::cout << " " << mid << " ";
                }
            }
            std::cout << "\n";
        }
    }

    // Modo clarividente: segunda pasada con anotaciones de efectos bajo la cuadrícula
    if (clairvoyant) {
        std::cout << "\n[Clarividente] Efectos en lineas:\n";
        // Lista algunos efectos de muestra encontrados
        int hRows = pointsR, hCols = pointsC - 1;
        // Revisa bordes horizontales
        for (int r = 0; r < hRows; ++r) {
            for (int c = 0; c < hCols; ++c) {
                Edge *e = H[idxH(r, c, hRows, hCols)];
                bool any = false;
                for (auto it = e->effects.iter(); it.has(); it.next()) {
                    if (it.val().active) {
                        // Solo efectos activos
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
        // Revisa bordes verticales
        for (int r = 0; r < vRows; ++r) {
            for (int c = 0; c < vCols; ++c) {
                Edge *e = V[idxV(r, c, vRows, vCols)];
                bool any = false;
                for (auto it = e->effects.iter(); it.has(); it.next()) {
                    if (it.val().active) {
                        // Solo efectos activos
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
