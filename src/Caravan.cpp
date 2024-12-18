#include "Caravan.h"
#include "Map.h"
#include <iostream>
using namespace std;

// Construtor da classe base Caravan
Caravan::Caravan(int id, const std::string& type, int maxCargo, int maxWater, int row, int col)
    : id(id), type(type), row(row), col(col), crew(10), cargo(0), maxCargo(maxCargo), water(maxWater), maxWater(maxWater) {}

// Atualiza a posição da caravana
void Caravan::setPosition(int newRow, int newCol) {
    row = newRow;
    col = newCol;
}

// Movimento baseado na direção
void Caravan::move(const std::string& direction) {
    if (water > 0) {
        // Consome água por movimento
        int waterConsumption = 2; // Exemplo de consumo (podes ajustar)
        water = std::max(0, water - waterConsumption);
    } else {
        // Quando a água é 0, os tripulantes começam a morrer
        loseCrew(1); // Perde 1 tripulante por movimento
        if (crew == 0) {
            std::cout << "[Caravana] ID: " << id << " está sem tripulação e tornou-se inativa!" << std::endl;
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
              << "Tripulacao restante: " << crew << std::endl
              << "Agua restante: " << water << "/" << maxWater << std::endl
              << "Carga atual: " << cargo << "/" << maxCargo << " toneladas" << std::endl;
}

// Construtor da classe TradeCaravan
TradeCaravan::TradeCaravan(int id) : Caravan(id, "Trade", 40, 200) {}

// Movimento especializado para TradeCaravan
void TradeCaravan::move(const std::string& direction) {
    cout << "[TradeCaravan] ID: " << id << " a mover-se para " << direction << "." << endl;
    Caravan::move(direction);
}

// Exibe o status da TradeCaravan
void TradeCaravan::status() const {
    cout << "[TradeCaravan] ID: " << id
         << ", Posicao: (" << row << ", " << col << ")"
         << "Tripulação restante: " << crew << "\n"
         << ", Carga: " << cargo << "/" << maxCargo
         << ", Agua: " << water << "/" << maxWater << endl;
}

// Construtor da classe MilitaryCaravan
MilitaryCaravan::MilitaryCaravan(int id) : Caravan(id, "Military", 5, 400) {}

// Movimento especializado para MilitaryCaravan
void MilitaryCaravan::move(const std::string& direction) {
    cout << "[MilitaryCaravan] ID: " << id << " a mover-se para " << direction << "." << endl;
    Caravan::move(direction);
}

// Exibe o status da MilitaryCaravan
void MilitaryCaravan::status() const {
    cout << "[MilitaryCaravan] ID: " << id
         << ", Posicao: (" << row << ", " << col << ")"
         << ", Tripulacao: " << crew
         << ", Agua: " << water << "/" << maxWater << endl;
}
