#pragma once
#include "../ds/linked_list.hpp"
#include "powerups.hpp"

enum class Orient { H, V };

struct LineEffect {
    PowerType type; // TS, BL, UF, AC
    int ownerId = -1;
    bool active = true;
    int roundIndexPlaced = -1;
};

struct Edge {
    int id = 0;
    int r = 0, c = 0; // grid index for H: between (r,c)-(r,c+1); for V: between (r,c)-(r+1,c)
    Orient o = Orient::H;

    bool placed = false;
    int placedBy = -1;
    int roundPlaced = -1;

    LinkedList<LineEffect> effects;

    bool hasActiveEffect(PowerType t, int *idxOut = nullptr) const {
        int i = 0;
        for (auto it = effects.iter(); it.has(); it.next(), ++i) {
            if (it.val().active && it.val().type == t) {
                if (idxOut) *idxOut = i;
                return true;
            }
        }
        return false;
    }
};
