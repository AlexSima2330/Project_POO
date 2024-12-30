#ifndef FIGHT_H
#define FIGHT_H

#include "Caravan.h"
#include "Simulator.h"

class Fight {
public:
    // Função principal para gerenciar um combate entre duas caravanas
    static void handleCombat(Caravan* userCaravan, Caravan* barbarianCaravan, Simulator* simulator);

    // Função para calcular o resultado de um combate entre duas caravanas
    static int rollCombatScore(int crew);

    // Função para aplicar as consequências após o combate
    static void applyCombatOutcome(Caravan* winner, Caravan* loser, Simulator* simulator, int winnerScore);

    // Função para verificar se uma caravana deve ser removida após o combate
    static void checkCaravanStatus(Caravan* caravan, Simulator* simulator);
};

#endif // FIGHT_H