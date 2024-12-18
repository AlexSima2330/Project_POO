#include "Simulator.h"
#include <iostream>

using namespace std;

void Simulator::addCaravan(Caravan* caravan, int row, int col) {
    if (map.getCell(row, col) == '.') {
        caravans.push_back(caravan);
        map.setCell(row, col, 'C');
    } else {
        cerr << "Erro: Posicao (" << row << ", " << col << ") ocupada ou invalida para a Caravana." << endl;
    }
}

void Simulator::showCaravanStatus() const {
    cout << "Estado das Caravanas:" << endl;
    for (auto caravan : caravans) {
        caravan->status();
    }
}

bool Simulator::moveCaravan(int caravanId, char direction) {
    for (auto caravan : caravans) {
        if (caravan->getId() == caravanId) {
            const int waterConsumptionPerMove = 10;

            if (!caravan->processMovement(map, waterConsumptionPerMove)) {
                buffer << "Caravana " << caravanId << " ficou inativa e tornou-se um obstáculo.\n";
                return false;
            }

            int oldRow = caravan->getRow();
            int oldCol = caravan->getCol();

            caravan->move(direction);
            auto [newRow, newCol] = map.wrapCoordinates(caravan->getRow(), caravan->getCol());

            char cellContent = map.getCell(newRow, newCol);
            if (cellContent == '+' || cellContent == 'b' || cellContent == 'a') {
                caravan->setPosition(oldRow, oldCol);
                buffer << "[Caravana] ID: " << caravanId << " nao pode ultrapassar obstaculos. Movimento invalido.\n";
                return false;
            }

            auto [wrappedOldRow, wrappedOldCol] = map.wrapCoordinates(oldRow, oldCol);
            if (map.getCell(wrappedOldRow, wrappedOldCol) == 'C') {
                map.setCell(wrappedOldRow, wrappedOldCol, caravan->wasOnCharger() ? 'c' : '.');
            }

            if (cellContent == 'c') {
                caravan->refillWater();
                caravan->setOnCharger(true);
                buffer << "Caravana " << caravanId << " reabasteceu agua no carregador em ("
                       << newRow << ", " << newCol << ").\n";
            } else {
                caravan->setOnCharger(false);
            }

            map.setCell(newRow, newCol, 'C');
            caravan->setPosition(newRow, newCol);

            buffer << "Caravana " << caravanId << " moveu-se para (" << newRow << ", " << newCol << ").\n";

            // Aqui chamas displayMap() para mostrar imediatamente a alteração.
            displayMap();

            return true;
        }
    }

    buffer << "Caravana com ID " << caravanId << " nao encontrada.\n";
    return false;
}

void Simulator::moveCaravanWithDirection(int caravanId, const std::string &direction) {
    if (direction.size() == 1) {
        // Se for apenas um caracter, chama moveCaravan diretamente.
        char dir = direction[0];
        moveCaravan(caravanId, dir);
    } else if (direction.size() == 2) {
        // Trata das direções compostas: CE, CD, BE, BD
        char dir1 = direction[0];
        char dir2 = direction[1];

        // Primeiro movimento
        moveCaravan(caravanId, dir1);
        // Segundo movimento
        moveCaravan(caravanId, dir2);

        // Não é necessário chamar displayMap() aqui, pois moveCaravan() já o faz.
    } else {
        // Caso a direção tenha mais de 2 caracteres, ou seja inválida
        std::cout << "Direção inválida: " << direction << std::endl;
    }
}

// Novos métodos relacionados a caravanas
void Simulator::buyCaravan(char cityName, char tipo) {
    cout << "Comprar caravana tipo " << tipo << " na cidade " << cityName << " (não implementado)." << endl;
}

void Simulator::showCaravanDetails(int caravanId) const {
    cout << "Mostrar detalhes da caravana " << caravanId << " (não implementado)." << endl;
}

void Simulator::buyMerchandise(int caravanId, int amount) {
    cout << "Comprar " << amount << " de mercadoria para caravana " << caravanId << " (não implementado)." << endl;
}

void Simulator::sellMerchandise(int caravanId) {
    cout << "Vender toda a mercadoria da caravana " << caravanId << " (não implementado)." << endl;
}

void Simulator::setCaravanAuto(int caravanId) {
    cout << "Caravana " << caravanId << " em modo auto (não implementado)." << endl;
}

void Simulator::stopCaravanAuto(int caravanId) {
    cout << "Caravana " << caravanId << " parou modo auto (não implementado)." << endl;
}

void Simulator::createBarbarianCaravan(int l, int c) {
    cout << "Criar caravana bárbara em (" << l << "," << c << ") (não implementado)." << endl;
}

void Simulator::addCrewToCaravan(int caravanId, int t) {
    cout << "Adicionar " << t << " tripulantes à caravana " << caravanId << " (não implementado)." << endl;
}
