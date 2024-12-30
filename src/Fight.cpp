#include "Fight.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

void Fight::handleCombat(Caravan* userCaravan, Caravan* barbarianCaravan, Simulator* simulator) {
    if (!userCaravan || !barbarianCaravan) {
        return;
    }

    cout << "[Combate] Caravana do Utilizador (ID: " << userCaravan->getId()
         << ") vs. Caravana Barbara " << endl;

    // Sorteio
    int userScore = rollCombatScore(userCaravan->getCrew());
    int barbarianScore = rollCombatScore(barbarianCaravan->getCrew());

    cout << "Resultado do Sorteio: Utilizador (" << userScore
         << ") vs. Barbaro (" << barbarianScore << ")" << endl;

    if (userScore > barbarianScore) {
        cout << "Caravana do Utilizador venceu o combate!" << endl;
        simulator->incrementCombatWins();
        applyCombatOutcome(userCaravan, barbarianCaravan, simulator, userScore);
    } else if (barbarianScore > userScore) {
        cout << "Caravana Barbara venceu o combate!" << endl;
        applyCombatOutcome(barbarianCaravan, userCaravan, simulator, barbarianScore);
    } else {
        cout << "Empate! Ambas as caravanas perderam alguns tripulantes." << endl << endl;
        userCaravan->loseCrew(static_cast<int>(floor(userScore * 0.2)));
        barbarianCaravan->loseCrew(static_cast<int>(floor(barbarianScore * 0.2)));
    }

    checkCaravanStatus(userCaravan, simulator);
    checkCaravanStatus(barbarianCaravan, simulator);
}

int Fight::rollCombatScore(int crew) {
    if (crew <= 0) return 0;
    return rand() % (crew + 1);
}

void Fight::applyCombatOutcome(Caravan* winner, Caravan* loser, Simulator* simulator, int winnerScore) {
    int winnerCrewLoss = static_cast<int>(floor(winnerScore * 0.2)); // Perde 20% da tripulação
    int loserCrewLoss = static_cast<int>(floor(winnerScore * 2));    // Perde o dobro do vencedor

    winner->loseCrew(winnerCrewLoss);
    loser->loseCrew(loserCrewLoss);

    if (winner->isOwned()) {
        cout << "[Caravana do Utilizador] ID: " << winner->getId()
             << " perdeu " << winnerCrewLoss << " tripulantes. Restantes: " << winner->getCrew() << endl;
    } else {
        cout << "[Caravana Barbara] perdeu " << winnerCrewLoss << " tripulantes. Restantes: " << winner->getCrew() << endl;
    }

    if (loser->isOwned()) {
        cout << "[Caravana do Utilizador] ID: " << loser->getId()
             << " perdeu " << loserCrewLoss << " tripulantes. Restantes: " << loser->getCrew() << endl;
    } else {
        cout << "[Caravana Barbara] perdeu " << loserCrewLoss << " tripulantes. Restantes: " << loser->getCrew() << endl;
    }

    if (winner->getCrew() <= 0) {
        cout << (winner->isOwned() ? "[Combate] Caravana do Utilizador ID: " + to_string(winner->getId()) : "[Combate] Caravana Barbara")
             << " foi destruida!" << endl;
        simulator->removeCaravan(winner);
    }

    if (loser->getCrew() <= 0) {
        cout << (loser->isOwned() ? "[Combate] Caravana do Utilizador ID: " + to_string(loser->getId()) : "[Combate] Caravana Barbara")
             << " foi destruida!" << endl;
        simulator->removeCaravan(loser);
    }
}

void Fight::checkCaravanStatus(Caravan* caravan, Simulator* simulator) {
    if (caravan->getCrew() <= 0) {
        cout << "[Combate] Caravana ID: " << caravan->getId()
             << " foi destruida apos o combate!" << endl;
        simulator->removeCaravan(caravan);
    }
}