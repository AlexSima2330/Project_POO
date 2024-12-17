#include "Caravan.h"
#include "Map.h"
#include <iostream>
using namespace std;

// Construtor da classe base Caravan
Caravan::Caravan(int id, const string &type, int maxCargo, int maxWater, int row, int col)
    : id(id), type(type), crew(10), cargo(0), water(maxWater), maxCargo(maxCargo), maxWater(maxWater), row(row), col(col) {}

// Atualiza a posição da caravana
void Caravan::setPosition(int newRow, int newCol) {
    row = newRow;
    col = newCol;
}

// Movimento baseado na direção
void Caravan::move(char direction) {
    switch (direction) {
        case 'B': ++row; break;
        case 'C': --row; break;
        case 'E': --col; break;
        case 'D': ++col; break;
        default:
            cout << "Direção inválida!" << endl;
    }
}

// Processa consumo de água e inatividade
bool Caravan::processMovement(Map &map, int waterConsumption) {
    if (water >= waterConsumption) {
        water -= waterConsumption;
        return true; // Movimento permitido
    }

    loseCrew(10); // Perde tripulantes por falta de água
    cout << "[Caravana] ID: " << id << " sem agua suficiente. Perdeu 2 tripulantes. Tripulantes restantes: " << crew << endl;

    // Se ficar inativa, transforma-se em obstáculo
    if (!isActive()) {
        becomeObstacle(map);
        return false;
    }
    return true; // Continua a tentar mover
}

// Transforma a caravana em obstáculo
void Caravan::becomeObstacle(Map &map) {
    auto [wrappedRow, wrappedCol] = map.wrapCoordinates(row, col);
    map.setCell(wrappedRow, wrappedCol, '+');
    cout << "[Caravana] ID: " << id << " tornou-se um obstaculo em (" << wrappedRow << ", " << wrappedCol << ")." << endl;
}

// Exibe o status básico
void Caravan::status() const {
    cout << "[Caravan] ID: " << id << " (" << type << ")"
              << ", Posicao: (" << row << ", " << col << ")"
              << ", Carga: " << cargo << "/" << maxCargo
              << ", Agua: " << water << "/" << maxWater
              << ", Tripulacao: " << crew << endl;
}

// Construtor da classe TradeCaravan
TradeCaravan::TradeCaravan(int id) : Caravan(id, "Trade", 40, 200) {}

// Movimento especializado para TradeCaravan
void TradeCaravan::move(char direction) {
    cout << "[TradeCaravan] ID: " << id << " a mover-se para " << direction << "." << endl;
    Caravan::move(direction); // Usa o movimento básico
}

// Exibe o status da TradeCaravan
void TradeCaravan::status() const {
    cout << "[TradeCaravan] ID: " << id
         << ", Posicao: (" << row << ", " << col << ")"
         << ", Carga: " << cargo << "/" << maxCargo
         << ", Agua: " << water << "/" << maxWater << endl;
}

// Construtor da classe MilitaryCaravan
MilitaryCaravan::MilitaryCaravan(int id) : Caravan(id, "Military", 5, 400) {}

// Movimento especializado para MilitaryCaravan
void MilitaryCaravan::move(char direction) {
    cout << "[MilitaryCaravan] ID: " << id << " a mover-se para " << direction << "." << endl;
    Caravan::move(direction); // Usa o movimento básico
}

// Exibe o status da MilitaryCaravan
void MilitaryCaravan::status() const {
    cout << "[MilitaryCaravan] ID: " << id
         << ", Posicao: (" << row << ", " << col << ")"
         << ", Tripulacao: " << crew
         << ", Agua: " << water << "/" << maxWater << endl;
}
