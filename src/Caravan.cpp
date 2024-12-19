#include "Caravan.h"
#include "Map.h"
#include <iostream>
using namespace std;

// Construtor da classe base Caravan
Caravan::Caravan(int id, const std::string &type, int row, int col)
    : id(id), type(type), row(row), col(col), cargo(0), water(0), maxCargo(0), maxWater(0), crew(0) {}

// Atualiza a posição da caravana
void Caravan::setPosition(int newRow, int newCol) {
    row = newRow;
    col = newCol;
}

// Movimento baseado na direção
void Caravan::move(const std::string& direction) {
    int waterConsumption = consumeWater();
    if (water >= waterConsumption) {
        water -= waterConsumption;
    } else {
        loseCrew(1);
        if (crew == 0) {
            std::cout << "[Caravan] ID: " << id << " esta sem tripulacao e tornou-se inativa!" << std::endl;
            return;
        }
    }

    // Movimento baseado na direção
    if (direction == "D") {
        ++col;
    } else if (direction == "E") {
        --col;
    } else if (direction == "C") {
        --row;
    } else if (direction == "B") {
        ++row;
    } else if (direction == "CE") {
        --row; --col;
    } else if (direction == "CD") {
        --row; ++col;
    } else if (direction == "BE") {
        ++row; --col;
    } else if (direction == "BD") {
        ++row; ++col;
    } else {
        std::cout << "Direção inválida: " << direction << std::endl;
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
    std::cout << "[Caravana] ID: " << id
              << " (" << type << ")" << std::endl
              << "Posicao: (" << row << ", " << col << ")" << std::endl
              << "Tripulacao atual: " << crew << std::endl
              << "Agua restante: " << water << "/" << maxWater << std::endl
              << "Carga atual: " << cargo << "/" << maxCargo << " toneladas" << std::endl;
}

// Construtor da classe TradeCaravan
TradeCaravan::TradeCaravan(int id, int initialCrew)
    : Caravan(id, "Trade") {
    maxCargo = 40;       // Capacidade máxima de carga para TradeCaravan
    maxWater = 200;      // Capacidade máxima de água
    water = maxWater;    // Inicia com tanque cheio
    crew = initialCrew;  // Número inicial de tripulantes
}

int TradeCaravan::consumeWater() const {
    return crew > 10 ? 3 : 2;
}

// Movimento especializado para TradeCaravan
void TradeCaravan::move(const std::string& direction) {
    cout << "[TradeCaravan] ID: " << id << " a mover-se para " << direction << "." << endl;
    Caravan::move(direction);
}


// Construtor da classe MilitaryCaravan
MilitaryCaravan::MilitaryCaravan(int id, int initialCrew)
    : Caravan(id, "Military") {
    maxCargo = 5;        // Capacidade máxima de carga para MilitaryCaravan
    maxWater = 400;      // Capacidade máxima de água
    water = maxWater;    // Inicia com tanque cheio
    crew = initialCrew;  // Número inicial de tripulantes
}

int MilitaryCaravan::consumeWater() const {
    return 5; // Sempre consome 5 por movimento
}

// Movimento especializado para MilitaryCaravan
void MilitaryCaravan::move(const std::string& direction) {
    cout << "[MilitaryCaravan] ID: " << id << " a mover-se para " << direction << "." << endl;
    Caravan::move(direction);
}