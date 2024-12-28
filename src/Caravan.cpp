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

    if (type == "Secret" && isCurrentlyInvisible()) {
        std::cout << "[Erro] Caravana Secreta esta invisivel e nao pode mover-se!" << std::endl;
        return;
    }

    int oldRow = row;
    int oldCol = col;

    // Movimento personalizado para Caravana Secreta
    if (type == "Secret") {
        if (direction == "2D") {
            col += 2; // Move duas casas para a direita
            return;
        } else if (direction == "2E") {
            col -= 2; // Move duas casas para a esquerda
            return;
        } else if (direction == "2C") {
            row -= 2; // Move duas casas para cima
            return;
        } else if (direction == "2B") {
            row += 2; // Move duas casas para baixo
            return;
        }
    }

    // Movimento padrão para todas as caravanas
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
        if (type == "Secret" && (direction == "2D" || direction == "2E" || direction == "2C" || direction == "2B")) {
            std::cout << "Direção invalida para Caravana Secreta: " << direction << std::endl;
        } else {
            std::cout << "Direção invalida: " << direction << std::endl;
        }
    }
}


// Processa consumo de água e inatividade
bool Caravan::processMovement(Map& map) {
    int waterConsumption = consumeWater();

    if (water >= waterConsumption) {
        water -= waterConsumption;
        return true; // Movimento permitido, água suficiente
    }

    // Se a água chegou a zero, verifica se é uma Caravana Secreta
    if (water <= 0) {
        if (type == "Secret") {
            becomeObstacle(map); // Torna-se um obstáculo no mapa
            return false; // Não pode mais se mover
        }
    }

    // Caso contrário, perde tripulantes apenas para outras caravanas
    loseCrew(1);

    return isActive(); // Movimento permitido apenas se ainda estiver ativa
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
    crew = initialCrew;
    cargo = 2;// Número inicial de tripulantes
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
    crew = initialCrew;
    cargo = 3;// Número inicial de tripulantes
}

SecretCaravan::SecretCaravan(int id, int initialCrew)
    : Caravan(id, "Secret") {
    maxCargo = 5;        // Capacidade máxima de carga para MilitaryCaravan
    maxWater = 400;      // Capacidade máxima de água
    water = maxWater;    // Inicia com tanque cheio
    crew = initialCrew;
    cargo = 3;// Número inicial de tripulantes
}

int SecretCaravan::consumeWater() const {
    return 1;
}
void SecretCaravan::handleNoWater(Map& map) {
    if (water <= 0) {
        std::cout << "[Caravana Secreta] ID: " << id << " ficou sem água e tornou-se um obstáculo permanente no mapa." << std::endl;
        becomeObstacle(map); // Torna-se um obstáculo permanente no mapa
    }
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

void SecretCaravan::move(const std::string& direction) {
    cout << "[MilitaryCaravan] ID: " << id << " a mover-se para " << direction << "." << endl;
    Caravan::move(direction);
}

bool Caravan::addCargo(int quantity) {
    if (cargo + quantity <= maxCargo) {
        cargo += quantity;
        return true; // Carga adicionada com sucesso
    }
    cout << "Erro: Carga excede a capacidade máxima da caravana." << endl;
    return false; // Falha ao adicionar carga
}

bool Caravan::removeCargo(int quantity) {
    if (quantity <= cargo) {
        cargo -= quantity;
        return true; // Carga removida com sucesso
    }
    cout << "Erro: Não é possível remover mais carga do que a atual." << endl;
    return false; // Falha ao remover carga
}

void Caravan::becomeObstacle(Map &map) {
    auto [wrappedRow, wrappedCol] = map.wrapCoordinates(row, col);
    map.setCell(wrappedRow, wrappedCol, '+'); // Marca como obstáculo no mapa
    cout << "[Caravana] ID: " << id << " tornou-se um obstaculo em (" << wrappedRow << ", " << wrappedCol << ")." << endl;
}

void Caravan::activateInvisibility() {
    if (type == "Secret" && !isInvisible) {
        isInvisible = true;
        invisibleTurns = 3; // A invisibilidade dura 3 turnos
        std::cout << "[Caravana Secreta] ID: " << id << " ativou o Modo Invisivel por 3 turnos!" << std::endl;
    } else if (isInvisible) {
        std::cout << "[Caravana Secreta] ID: " << id << " já está invisivel!" << std::endl;
    }
}

void Caravan::updateInvisibility() {
    if (isInvisible) {
        invisibleTurns--;
        if (invisibleTurns <= 0) {
            isInvisible = false;
            std::cout << "[Caravana Secreta] ID: " << id << " saiu do Modo Invisível!" << std::endl;
        }
    }
}
