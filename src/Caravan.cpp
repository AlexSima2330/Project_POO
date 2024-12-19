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
bool Caravan::processMovement(Map& map) {
    int waterConsumption = consumeWater(); // Consumo de água específico da subclasse
    if (water >= waterConsumption) {
        water -= waterConsumption; // Reduz a água disponível
        return true; // Movimento permitido
    }

    loseCrew(1); // Perde 1 tripulante por falta de água
    std::cout << "[Caravana] ID: " << id << " sem água suficiente. Perdeu 1 tripulante. Tripulantes restantes: " << crew << std::endl;

    if (!isActive()) {
        return false; // Movimento não permitido se não houver tripulantes
    }

    return true; // Movimento permitido, mas com penalização
}


// Exibe o status básico
void Caravan::status() const {
    std::cout << "[Caravana] ID: " << id
              << " (" << type << ")"
              << ", Posicao: (" << row << ", " << col << ")"
              << ", Tripulacao atual: " << crew
              << ", Carga atual: " << cargo << "/" << maxCargo << "T"
              << ", Agua atual: " << water << "/" << maxWater  << std::endl;
}

// Construtor da classe TradeCaravan
TradeCaravan::TradeCaravan(int id, int initialCrew)
    : Caravan(id, "Trade") {
    maxCargo = 5;       // Capacidade máxima de carga para TradeCaravan
    maxWater = 400;      // Capacidade máxima de água
    water = maxWater;    // Inicia com tanque cheio
    crew = initialCrew;  // Número inicial de tripulantes
}

int TradeCaravan::consumeWater() const {
    if (crew == 0) {
        return 0; // Sem tripulantes, não consome água
    } else if (crew <= 10) {
        return 1; // Metade ou menos dos tripulantes
    } else {
        return 2; // Mais de 10 tripulantes
    }
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
    if (crew == 0 || crew <= 10) {
        return 1; // Sem tripulantes ou metade ou menos dos tripulantes
    } else {
        return 3; // Mais de 10 tripulantes
    }
}

// Movimento especializado para MilitaryCaravan
void MilitaryCaravan::move(const std::string& direction) {
    cout << "[MilitaryCaravan] ID: " << id << " a mover-se para " << direction << "." << endl;
    Caravan::move(direction);
}