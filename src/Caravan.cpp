#include "Caravan.h"
#include "Map.h"
#include <iostream>
using namespace std;

Caravan::Caravan(int id, const std::string &type, int row, int col)
    : id(id), type(type), row(row), col(col), cargo(0), water(0), maxCargo(0), maxWater(0), crew(0) {}

void Caravan::setPosition(int newRow, int newCol) {
    row = newRow;
    col = newCol;
}

void Caravan::move(const std::string& direction) {
    if (type == "Military") {
        lastDirection = direction;
    }

    if (type == "Secret" && isCurrentlyInvisible()) {
        std::cout << "[Erro] Caravana Secreta esta invisivel e nao pode mover-se!" << std::endl;
        return;
    }

    if (type == "Secret") {
        if (direction == "2D") {
            col += 2;
            return;
        } else if (direction == "2E") {
            col -= 2;
            return;
        } else if (direction == "2C") {
            row -= 2;
            return;
        } else if (direction == "2B") {
            row += 2;
            return;
        }
    }

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
            std::cout << "Direcao invalida para Caravana Secreta: " << direction << std::endl;
        } else {
            std::cout << "Direcao invalida: " << direction << std::endl;
        }
    }
}

bool Caravan::processMovement(Map& map) {
    if (crew == 0) {
        return true;
    }

    int waterConsumption = consumeWater();
    if (water >= waterConsumption) {
        water -= waterConsumption;
        return true;
    }

    if (type == "Secret" && water <= 0) {
        becomeObstacle(map);
        return false;
    }

    loseCrew(1);
    return isActive();
}

void Caravan::status() const {
    std::cout << "[Caravana] ID: " << id
              << " (" << type << ")"
              << ", Posicao: (" << row << ", " << col << ")"
              << ", Tripulacao atual: " << crew
              << ", Carga atual: " << cargo << "/" << maxCargo << "T"
              << ", Agua atual: " << water << "/" << maxWater  << std::endl;
}

TradeCaravan::TradeCaravan(int id, int initialCrew)
    : Caravan(id, "Trade") {
    maxCargo = 40;
    maxWater = 200;
    water = maxWater;
    crew = initialCrew;
    cargo = 0;
}

int TradeCaravan::consumeWater() const {
    if (crew == 0) {
        return 0;
    } else if (crew <= 10) {
        return 1;
    } else {
        return 2;
    }
}

void TradeCaravan::move(const std::string& direction) {
    cout << "[TradeCaravan] ID: " << id << " a mover-se para " << direction << "." << endl;
    Caravan::move(direction);
}

MilitaryCaravan::MilitaryCaravan(int id, int initialCrew)
    : Caravan(id, "Military") {
    maxCargo = 5;
    maxWater = 400;
    water = maxWater;
    crew = initialCrew;
    cargo = 0;
}

SecretCaravan::SecretCaravan(int id, int initialCrew)
    : Caravan(id, "Secret") {
    maxCargo = 10;
    maxWater = 250;
    water = maxWater;
    crew = initialCrew;
    cargo = 0;
}

int SecretCaravan::consumeWater() const {
    return 1;
}
void SecretCaravan::handleNoWater(Map& map) {
    if (water <= 0) {
        std::cout << "[Caravana Secreta] ID: " << id << " ficou sem agua e tornou-se um obstaculo no mapa." << std::endl;
        becomeObstacle(map);
    }
}

int MilitaryCaravan::consumeWater() const {
    if (crew == 0 || crew <= 10) {
        return 1;
    } else {
        return 3;
    }
}

void MilitaryCaravan::move(const std::string& direction) {
    cout << "[MilitaryCaravan] ID: " << id << " a mover-se para " << direction << "." << endl;
    Caravan::move(direction);
}

void SecretCaravan::move(const std::string& direction) {
    cout << "[SecretCaravan] ID: " << id << " a mover-se para " << direction << "." << endl;
    Caravan::move(direction);
}

bool Caravan::addCargo(int quantity) {
    if (cargo + quantity <= maxCargo) {
        cargo += quantity;
        return true;
    }
    cout << "Erro: Carga excede a capacidade maxima da caravana." << endl;
    return false;
}

bool Caravan::removeCargo(int quantity) {
    if (quantity <= cargo) {
        cargo -= quantity;
        return true;
    }
    cout << "Erro: Nao e possivel remover mais carga do que a atual." << endl;
    return false;
}

void Caravan::becomeObstacle(Map &map) {
    auto [wrappedRow, wrappedCol] = map.wrapCoordinates(row, col);
    map.setCell(wrappedRow, wrappedCol, '+');
    cout << "[Caravana] ID: " << id << " tornou-se um obstaculo em (" << wrappedRow << ", " << wrappedCol << ")." << endl;
}

void Caravan::activateInvisibility() {
    if (type == "Secret" && !isInvisible) {
        isInvisible = true;
        invisibleTurns = 3;
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

void Caravan::moveRandomly(Map& map) {
    std::vector<std::string> directions = {"C", "B", "D", "E", "CE", "CD", "BE", "BD"};
    std::string randomDirection = directions[rand() % directions.size()];
    int oldRow = row;
    int oldCol = col;

    move(randomDirection);

    auto [newRow, newCol] = map.wrapCoordinates(row, col);

    if (map.getCell(newRow, newCol) == '.') {
        map.setCell(oldRow, oldCol, '.');
        map.setCell(newRow, newCol, '0' + id);
        row = newRow;
        col = newCol;

        std::cout << "[TradeCaravan] ID: " << id << " moveu-se aleatoriamente para ("
                  << newRow << ", " << newCol << ")." << std::endl;
    } else {
        row = oldRow;
        col = oldCol;
        std::cout << "[TradeCaravan] ID: " << id
                  << " nao conseguiu mover-se aleatoriamente devido a um obstaculo." << std::endl;
    }
}

void Caravan::moveInLastDirection(Map& map) {
    if (!lastDirection.empty()) {
        int oldRow = row;
        int oldCol = col;

        move(lastDirection);

        auto [newRow, newCol] = map.wrapCoordinates(row, col);

        if (map.getCell(newRow, newCol) == '.') {
            map.setCell(oldRow, oldCol, '.');
            map.setCell(newRow, newCol, '0' + id);
            row = newRow;
            col = newCol;

            std::cout << "[MilitaryCaravan] ID: " << id
                      << " moveu-se na ultima direcao conhecida para ("
                      << newRow << ", " << newCol << ")." << std::endl;
        } else {
            row = oldRow;
            col = oldCol;
            std::cout << "[MilitaryCaravan] ID: " << id
                      << " nao conseguiu mover-se devido a um obstaculo." << std::endl;
        }
    } else {
        std::cout << "[MilitaryCaravan] ID: " << id
                  << " nao tem uma última direcao valida para se mover." << std::endl;
    }
}