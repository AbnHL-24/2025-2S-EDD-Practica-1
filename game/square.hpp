#pragma once
#include "edge.hpp"
#include "powerups.hpp"

struct Square {
    int r = 0, c = 0; // cell coordinates
    Edge *top = nullptr;
    Edge *right = nullptr;
    Edge *bottom = nullptr;
    Edge *left = nullptr;

    bool closed = false;
    int ownerId = -1; // visual owner (initial in cell)
    int pointsAwardedTo = -1; // who got the point(s)

    bool hasPower = false;
    PowerUp power{PowerType::None};

    bool isClosable() const {
        return top && right && bottom && left &&
               top->placed && right->placed && bottom->placed && left->placed;
    }
};
