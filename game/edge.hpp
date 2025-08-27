#pragma once
#include "../ds/linked_list.hpp"
#include "powerups.hpp"

enum class Orient { H, V };

struct LineEffect { PowerType type; int ownerId; bool active=true; int roundIndexPlaced=-1; };

struct Edge {
    int id=0; int r=0, c=0; Orient o=Orient::H;
    bool placed=false; int placedBy=-1; int roundPlaced=-1;
    LinkedList<LineEffect> effects;
    bool hasEffect(PowerType t, int* idxOut=nullptr){ int i=0; for(auto it=effects.iter(); it.has(); it.next(),++i){ if(it.val().type==t && it.val().active){ if(idxOut)*idxOut=i; return true; } } return false; }
};