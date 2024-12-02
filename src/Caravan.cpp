#include "Caravan.h"
#include <iostream>

// Construtor da classe base Caravan
Caravan::Caravan(int id, const std::string &type, int maxCargo, int maxWater, int row, int col)
    : id(id), type(type), crew(10), cargo(0), water(maxWater), maxCargo(maxCargo), maxWater(maxWater), row(row), col(col) {}

// Atualiza a posição da caravana
void Caravan::setPosition(int newRow, int newCol) {
    row = newRow;
    col = newCol;
}

// Movimento consoante a agua
void Caravan::move(char direction) {
    // Consome água antes de mover
    const int waterConsumptionPerMove = 10; // Define o consumo padrão
    if (water >= waterConsumptionPerMove) {
        water -= waterConsumptionPerMove; // Reduz a água
    } else {
        std::cout << "Caravana " << id << " sem agua suficiente para mover!" << std::endl;
        return;
    }

    // Atualiza a posição conforme a direção
    switch (direction) {
        case 'B': // Baixo
            ++row;
        break;
        case 'C': // Cima
            --row;
        break;
        case 'E': // Esquerda
            --col;
        break;
        case 'D': // Direita
            ++col;
        break;
        default:
            std::cout << "Direcao invalida!" << std::endl;
        break;
    }
}


// Exibe o status básico
void Caravan::status() const {
    std::cout << "[Caravan] ID: " << id << " (" << type << ")"
              << ", Posicao: (" << row << ", " << col << ")"
              << ", Carga: " << cargo << "/" << maxCargo
              << ", Agua: " << water << "/" << maxWater
              << ", Tripulacao: " << crew << std::endl;
}

// Construtor da classe TradeCaravan
TradeCaravan::TradeCaravan(int id) : Caravan(id, "Trade", 40, 200) {}

// Movimento especializado para TradeCaravan
void TradeCaravan::move(char direction) {
    std::cout << "[TradeCaravan] ID: " << id << " a mover-se para " << direction << "." << std::endl;
    Caravan::move(direction); // Usa o movimento básico
}

// Exibe o status da TradeCaravan
void TradeCaravan::status() const {
    std::cout << "[TradeCaravan] ID: " << id
              << ", Posicao: (" << row << ", " << col << ")"
              << ", Carga: " << cargo << "/" << maxCargo
              << ", Agua: " << water << "/" << maxWater << std::endl;
}

void Caravan::addResource() {
    if (cargo < maxCargo) {
        ++cargo; // Incrementa a carga
        std::cout << "Caravana " << id << " coletou 1 recurso. Carga atual: " << cargo << "/" << maxCargo << std::endl;
    } else {
        std::cout << "Caravana " << id << " está com a carga maxima!" << std::endl;
    }
}


// Construtor da classe MilitaryCaravan
MilitaryCaravan::MilitaryCaravan(int id) : Caravan(id, "Military", 5, 400) {}

// Movimento especializado para MilitaryCaravan
void MilitaryCaravan::move(char direction) {
    std::cout << "[MilitaryCaravan] ID: " << id << " a mover-se para " << direction << "." << std::endl;
    Caravan::move(direction); // Usa o movimento básico
}

// Exibe o status da MilitaryCaravan
void MilitaryCaravan::status() const {
    std::cout << "[MilitaryCaravan] ID: " << id
              << ", Posicao: (" << row << ", " << col << ")"
              << ", Tripulacao: " << crew
              << ", Agua: " << water << "/" << maxWater << std::endl;
}
