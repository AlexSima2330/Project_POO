#ifndef BARBARIAN_CARAVAN_H
#define BARBARIAN_CARAVAN_H

#include "Caravan.h"

class BarbarianCaravan : public Caravan {
private:
    int spawnTurn = 0;
public:
    BarbarianCaravan(int id)
        : Caravan(id, "Barbarian", 0, 0) { // Sem capacidade de carga e água
        crew = 40; // 40 bárbaros armados
    }

    void move(const std::string& direction) override {
        cout << "[BarbarianCaravan] ID: " << id << " moveu-se na direção " << direction << "." << endl;
        Caravan::move(direction); // Movimento básico herdado
    }

    void status() const override {
        cout << "[BarbarianCaravan] ID: " << id
             << ", Posição: (" << row << ", " << col << ")"
             << ", Bárbaros: " << crew << endl;
    }

    int consumeWater() const override {
        return 0; // Bárbaros não consomem água
    }

    int getMaxMovesPerTurn() const override {
        return 1; // Bárbaros podem se mover 1 vez por turno
    }

    void setSpawnTurn(int turn) {
        if (turn >= 0) {
            spawnTurn = turn;
        } else {
            cout << "[Erro] Turno de spawn inválido para a caravana bárbara." << endl;
        }
    }

    int getSpawnTurn() const { return spawnTurn; }
};

#endif
