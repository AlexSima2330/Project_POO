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

            // Converter char para std::string
            std::string directionStr(1, direction);
            caravan->move(directionStr);

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
                buffer << "Caravana " << caravanId << " reabasteceu agua no carregador em (" << newRow << ", " << newCol << ").\n";
            } else {
                caravan->setOnCharger(false);
            }

            map.setCell(newRow, newCol, 'C');
            caravan->setPosition(newRow, newCol);

            buffer << "Caravana " << caravanId << " moveu-se para (" << newRow << ", " << newCol << ").\n";

            return true;
        }
    }

    buffer << "Caravana com ID " << caravanId << " nao encontrada.\n";
    return false;
}

void Simulator::moveCaravanWithDirection(int caravanId, const std::string& direction) {
    for (auto caravan : caravans) {
        if (caravan->getId() == caravanId) {
            int oldRow = caravan->getRow();
            int oldCol = caravan->getCol();

            // Calcular as coordenadas do movimento
            caravan->move(direction);
            auto [newRow, newCol] = map.wrapCoordinates(caravan->getRow(), caravan->getCol());

            // Verificar o conteúdo da célula de destino
            char cellContent = map.getCell(newRow, newCol);
            if (cellContent == '+') {
                std::cout << "[Erro] Movimento invalido devido a um obstaculo!" << std::endl;
                caravan->setPosition(oldRow, oldCol); // Restaura a posição antiga
                return;
            }

            // Atualizar o mapa
            map.setCell(oldRow, oldCol, '.'); // Marca a célula anterior como vazia
            map.setCell(newRow, newCol, 'C'); // Marca a nova célula com a caravana

            std::cout << "Caravana ID " << caravanId << " moveu-se para (" << newRow << ", " << newCol << ")." << std::endl;

            return;
        }
    }
    std::cout << "[Erro] Caravana com ID " << caravanId << " não encontrada." << std::endl;
}


// Novos métodos relacionados a caravanas
void Simulator::buyCaravan(char cityName, char tipo) {
    cout << "Comprar caravana tipo " << tipo << " na cidade " << cityName << " (não implementado)." << endl;
}

void Simulator::showCaravanDetails(int caravanId) const {
    for (const auto& caravan : caravans) {
        if (caravan->getId() == caravanId) {
            caravan->status();
            return;
        }
    }
    std::cout << "Caravana com ID " << caravanId << " não encontrada." << std::endl;
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
