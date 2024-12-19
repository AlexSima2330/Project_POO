#include "Simulator.h"
#include <iostream>

using namespace std;

void Simulator::addCaravan(Caravan* caravan, int row, int col) {
    if (map.getCell(row, col) == '.') {
        caravans.push_back(caravan);
        map.setCell(row, col, '0' + caravan->getId()); // Insere o ID da caravana como char
    } else {
        cerr << "Erro: Posicao (" << row << ", " << col << ") ocupada ou invalida para a Caravana." << endl;
    }
}

bool Simulator::moveCaravan(int caravanId, char direction) {
    for (auto caravan : caravans) {
        if (caravan->getId() == caravanId) {
            // Processa o consumo de água antes do movimento
            if (!caravan->processMovement(map)) {
                std::cout << "Caravana " << caravanId << " não pode mover-se devido à falta de água ou tripulantes." << std::endl;
                return false;
            }

            // Executa o movimento
            std::string directionStr(1, direction);
            caravan->move(directionStr);

            // Atualiza a posição no mapa
            int newRow = caravan->getRow();
            int newCol = caravan->getCol();
            map.setCell(newRow, newCol, 'C');

            std::cout << "Caravana " << caravanId << " moveu-se para (" << newRow << ", " << newCol << ")." << std::endl;
            return true;
        }
    }
    std::cout << "Caravana com ID " << caravanId << " não encontrada." << std::endl;
    return false;
}

void Simulator::moveCaravanWithDirection(int caravanId, const std::string& direction) {
    for (auto caravan : caravans) {
        if (caravan->getId() == caravanId) {
            int oldRow = caravan->getRow();
            int oldCol = caravan->getCol();

            // Processa o movimento
            if (!caravan->processMovement(map)) {
                std::cout << "[Erro] Caravana " << caravanId << " não pode mover-se devido a falta de água." << std::endl;
                return;
            }

            caravan->move(direction);
            auto [newRow, newCol] = map.wrapCoordinates(caravan->getRow(), caravan->getCol());

            // Verifica se o destino é válido
            char cellContent = map.getCell(newRow, newCol);
            if (cellContent != '.') { // Verifica se a célula está vazia
                std::cout << "[Erro] Movimento inválido para posição ocupada ou obstáculo!" << std::endl;
                caravan->setPosition(oldRow, oldCol);
                return;
            }

            // Atualiza o mapa
            map.setCell(oldRow, oldCol, '.'); // Limpa a célula antiga
            map.setCell(newRow, newCol, '0' + caravanId); // Atualiza com o ID da caravana

            // Exibe o status atualizado
            caravan->status();

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
