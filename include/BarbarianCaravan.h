#ifndef BARBARIAN_CARAVAN_H
#define BARBARIAN_CARAVAN_H

#include <iostream>

#include "Caravan.h"

class BarbarianCaravan : public Caravan {
private:
    int spawnTurn = 0;
public:
    BarbarianCaravan(int id)
        : Caravan(id, "Barbarian", 0, 0) {
        crew = 40;
    }

    void move(const std::string& direction) override {
        cout << "[BarbarianCaravan] " << id << " moveu-se na direcao " << direction << "." << endl;
        Caravan::move(direction);
    }

    void status() const override {
        cout << "[BarbarianCaravan] ID: " << id
             << ", Posicao: (" << row << ", " << col << ")"
             << ", Barbaros: " << crew << endl;
    }

    int consumeWater() const override {
        return 0;
    }

    int getMaxMovesPerTurn() const override {
        return 1;
    }

    void setSpawnTurn(int turn) {
        if (turn >= 0) {
            spawnTurn = turn;
        } else {
            cout << "[Erro] Turno de spawn invalido para a caravana barbara." << endl;
        }
    }

    int getSpawnTurn() const { return spawnTurn; }
};

#endif
