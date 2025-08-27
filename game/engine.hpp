#pragma once
#include "../ds/linked_list.hpp"
#include "board.hpp"
#include "player.hpp"
#include "turn_manager.hpp"
#include "powerups.hpp"

class Engine {
    Board board;
    LinkedList<Player *> players;
    TurnManager tm;

    bool clairvoyant = false;

public:
    ~Engine();

    void setupInteractively();

    void gameLoop();

private:
    Player *getPlayer(int id) const;

    void renderBoard();

    void printScores();

    // Actions
    void actionPlaceLine();

    void actionUsePower();

    // Internal rules
    void onLinePlaced(Edge *e, Player *p);

    void resolveSquaresForEdge(Edge *e, Player *p);

    // Helpers
    void pushPower(Player *p, PowerType t);

    bool isGameFinished() const;
};
