#ifndef FIGHT_H
#define FIGHT_H

#include "Simulator.h"

class Fight {
public:
    static void handleCombat(Caravan* userCaravan, Caravan* barbarianCaravan, Simulator* simulator);

    static int rollCombatScore(int crew);

    static void applyCombatOutcome(Caravan* winner, Caravan* loser, Simulator* simulator, int winnerScore);

    static void checkCaravanStatus(Caravan* caravan, Simulator* simulator);
};

#endif