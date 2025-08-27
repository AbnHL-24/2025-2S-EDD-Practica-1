#pragma once
#include "../ds/linked_list.hpp"
#include "powerups.hpp"

// Enumeración que define la orientación de un borde/línea
enum class Orient {
    H, // Horizontal - línea horizontal
    V // Vertical - línea vertical
};

// Estructura que representa un efecto aplicado a una línea/borde
struct LineEffect {
    PowerType type; // Tipo de power-up del efecto (TS, BL, UF, AC)
    int ownerId = -1; // ID del jugador propietario del efecto
    bool active = true; // Indica si el efecto está activo
    int roundIndexPlaced = -1; // Índice de la ronda en que se colocó el efecto
};

// Estructura que representa un borde/línea en la cuadrícula del juego
struct Edge {
    int id = 0; // Identificador único del borde
    int r = 0, c = 0; // Índices de cuadrícula: para H entre (r,c)-(r,c+1); para V entre (r,c)-(r+1,c)
    Orient o = Orient::H; // Orientación del borde (horizontal o vertical)

    bool placed = false; // Indica si el borde ha sido colocado/dibujado
    int placedBy = -1; // ID del jugador que colocó el borde
    int roundPlaced = -1; // Ronda en que se colocó el borde

    LinkedList<LineEffect> effects; // Lista de efectos activos en este borde

    // Verifica si el borde tiene un efecto activo del tipo especificado
    bool hasActiveEffect(PowerType t, int *idxOut = nullptr) const {
        int i = 0;
        for (auto it = effects.iter(); it.has(); it.next(), ++i) {
            // Itera sobre todos los efectos
            if (it.val().active && it.val().type == t) {
                // Si encuentra efecto activo del tipo buscado
                if (idxOut) *idxOut = i; // Opcionalmente retorna el índice
                return true; // Confirma que existe el efecto
            }
        }
        return false; // No se encontró efecto activo del tipo
    }
};
