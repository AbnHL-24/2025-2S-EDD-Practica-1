#pragma once
#include "edge.hpp"
#include "powerups.hpp"

struct Square {
    int r=0, c=0;
    Edge* top=nullptr; Edge* right=nullptr; Edge* bottom=nullptr; Edge* left=nullptr;
    bool closed=false; int ownerId=-1; int pointsAwardedTo=-1;
    bool hasPower=false; PowerUp power;
    bool isClosable() const { return top && right && bottom && left && top->placed && right->placed && bottom->placed && left->placed; }
};