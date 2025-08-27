#pragma once
#include <string>
#include "../ds/stack.hpp"
#include "powerups.hpp"

struct Player {
    int id=0; std::string name; char initial='?';
    int scorePoints=0; int squaresOwned=0; int powerUpsUsed=0;
    bool hasES=false; bool hasLS=false; bool hasDL=false; int dlRemaining=0;
    Stack<PowerUp> powerStack;
};