#include "Simulator.h"
#include "BarbarianCaravan.h"
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

            // Verifica se a caravana está numa cidade e remove-a se estiver
            if (caravan->isInCity()) {
                City* city = map.getCityAt(oldRow, oldCol);
                if (city) {
                    city->removeCaravan(caravan);
                    caravan->setInCity(false);
                    std::cout << "Caravana " << caravanId << " saiu da cidade " << city->getName() << "." << std::endl;
                }
            }

            // Processa o movimento
            if (!caravan->processMovement(map)) {
                std::cout << "[Erro] Caravana " << caravanId << " não pode mover-se devido a falta de água." << std::endl;
                return;
            }

            caravan->move(direction);
            auto [newRow, newCol] = map.wrapCoordinates(caravan->getRow(), caravan->getCol());

            // Verifica se o destino é um obstáculo
            char destinationCell = map.getCell(newRow, newCol);
            if (destinationCell == '+') {
                std::cout << "[Erro] Movimento inválido: destino contém um obstáculo." << std::endl;

                // Restaura a posição anterior da caravana
                caravan->setPosition(oldRow, oldCol);
                return;
            }

            // Verifica se o destino já contém uma caravana
            if (std::isdigit(destinationCell)) {
                std::cout << "[Erro] Movimento inválido: destino já contém outra caravana." << std::endl;

                // Restaura a posição anterior da caravana
                caravan->setPosition(oldRow, oldCol);
                return;
            }

            // Verifica se o destino é uma cidade
            if (map.isCity(newRow, newCol)) {
                City* city = map.getCityAt(newRow, newCol);
                if (city) {
                    city->addCaravan(caravan);
                    caravan->setInCity(true);
                    std::cout << "Caravana " << caravanId << " entrou na cidade " << city->getName() << "." << std::endl;

                    // A célula antiga é limpa, pois a caravana entrou na cidade
                    if (!map.isCity(oldRow, oldCol)) {
                        map.setCell(oldRow, oldCol, '.');
                    }

                    return; // Não adiciona a caravana no mapa visual
                }
            }

            // Atualiza o mapa
            // Limpa a célula antiga se não for uma cidade
            if (!map.isCity(oldRow, oldCol)) {
                map.setCell(oldRow, oldCol, '.');
            }

            // Atualiza a nova posição com o ID da caravana
            map.setCell(newRow, newCol, '0' + caravanId);
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
    // Verifica se a posição é válida e desocupada
    if (map.getCell(l, c) == '.') {
        Caravan* barbarian = new BarbarianCaravan(caravans.size() + 1); // ID único
        barbarian->setPosition(l, c);
        caravans.push_back(barbarian); // Adiciona à lista de caravanas
        map.setCell(l, c, '!'); // Marca no mapa com o símbolo de caravana bárbara
        cout << "Caravana bárbara criada na posição (" << l << ", " << c << ")." << endl;
    } else {
        cerr << "Erro: Não é possível criar uma caravana bárbara em (" << l << ", " << c << "). Posição inválida ou ocupada." << endl;
    }
}


void Simulator::addCrewToCaravan(int caravanId, int t) {
    cout << "Adicionar " << t << " tripulantes à caravana " << caravanId << " (não implementado)." << endl;
}
