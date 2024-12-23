#include "Simulator.h"
#include "BarbarianCaravan.h"
#include <iostream>
#include <algorithm>

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
                    caravan->refillWater();
                    std::cout << "Caravana " << caravanId << " entrou na cidade " << city->getName()
                              << " e foi reabastecida de água." << std::endl;

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

void Simulator::buyMerchandise(int caravanId, int quantity) {
    for (auto& caravan : caravans) {
        if (caravan->getId() == caravanId) {
            if (!caravan->isInCity()) {
                cout << "Erro: A caravana " << caravanId << " precisa estar numa cidade para comprar mercadorias." << endl;
                return;
            }

            int cost = quantity; // Preço por tonelada é 1 moeda
            if (wallet.getCoins() < cost) {
                cout << "Erro: Moedas insuficientes para comprar " << quantity << " toneladas." << endl;
                return;
            }

            if (caravan->addCargo(quantity)) {
                wallet.addCoins(-cost);
                cout << "Caravana " << caravanId << " comprou " << quantity
                     << " toneladas de mercadoria. Moedas restantes: " << wallet.getCoins() << "." << endl;
            }
            return;
        }
    }
    cout << "Erro: Caravana " << caravanId << " não encontrada." << endl;
}

void Simulator::sellMerchandise(int caravanId) {
    for (auto& caravan : caravans) {
        if (caravan->getId() == caravanId) {
            if (!caravan->isInCity()) {
                cout << "Erro: A caravana " << caravanId << " precisa estar numa cidade para vender mercadorias." << endl;
                return;
            }

            int revenue = caravan->getCargo() * 2; // Preço de venda por tonelada
            caravan->setCargo(0); // Esvazia a carga
            wallet.addCoins(revenue);
            cout << "Caravana " << caravanId << " vendeu toda a mercadoria por " << revenue
                 << " moedas. Moedas totais: " << wallet.getCoins() << "." << endl;
            return;
        }
    }
    cout << "Erro: Caravana " << caravanId << " nao encontrada." << endl;
}


void Simulator::setCaravanAuto(int caravanId) {
    for (auto& caravan : caravans) {
        if (caravan->getId() == caravanId) {
            caravan->setAuto(true);
            std::cout << "Caravana " << caravanId << " esta agora em modo automatico." << std::endl;

            // Processa imediatamente o movimento automático
            advanceSimulation(1);
            return;
        }
    }
    std::cout << "Erro: Caravana com ID " << caravanId << " nao encontrada." << std::endl;
}


void Simulator::handleTradeCaravanAuto(Caravan* caravan) {
    // Exemplo: Se estiver próximo de outra caravana ou item
    int row = caravan->getRow();
    int col = caravan->getCol();

    // Buscar caravanas amigas ou itens próximos
    for (auto& otherCaravan : caravans) {
        if (otherCaravan != caravan && abs(otherCaravan->getRow() - row) <= 2 &&
            abs(otherCaravan->getCol() - col) <= 2) {
            // Tenta mover para uma posição adjacente amigável
            std::cout << "Caravana de Comercio " << caravan->getId()
                      << " movendo-se para proteçao ou pegar itens." << std::endl;
            return;
            }
    }
}
void Simulator::handleMilitaryCaravanAuto(Caravan* caravan) {
    if (caravan->getType() != "Military") return;

    int caravanRow = caravan->getRow();
    int caravanCol = caravan->getCol();

    for (auto& otherCaravan : caravans) {
        if (otherCaravan->getType() == "Barbarian" && otherCaravan->isActive()) {
            int barbarianRow = otherCaravan->getRow();
            int barbarianCol = otherCaravan->getCol();

            // Verifica se estão adjacentes
            if (areAdjacent(caravanRow, caravanCol, barbarianRow, barbarianCol)) {
                engageCombat(caravan, otherCaravan);
                return; // Sai após o combate
            }

            // Movimento em direção ao bárbaro
            if (abs(barbarianRow - caravanRow) <= 6 && abs(barbarianCol - caravanCol) <= 6) {
                map.setCell(caravanRow, caravanCol, '.'); // Limpa a posição antiga

                if (barbarianRow > caravanRow) caravan->move("B");
                else if (barbarianRow < caravanRow) caravan->move("C");
                if (barbarianCol > caravanCol) caravan->move("D");
                else if (barbarianCol < caravanCol) caravan->move("E");

                // Atualiza o mapa com a nova posição
                map.setCell(caravan->getRow(), caravan->getCol(), '0' + caravan->getId());
                return;
            }
        }
    }

    std::cout << "Caravana Militar " << caravan->getId() << " não encontrou bárbaros próximos." << std::endl;
}




void Simulator::handleCaravanWithoutCrew(Caravan* caravan) {
    caravan->incrementAutoTurnsWithoutCrew();

    if (caravan->getAutoTurnsWithoutCrew() > (caravan->getType() == "Trade" ? 5 : 7)) {
        std::cout << "Caravana " << caravan->getId() << " desapareceu por falta de tripulantes." << std::endl;
        removeCaravan(caravan);
        return;
    }

    // Movimento baseado no tipo
    if (caravan->getType() == "Trade") {
        std::cout << "Caravana de Comercio " << caravan->getId()
                  << " movendo-se de forma aleatória sem tripulacao." << std::endl;
        caravan->move("random"); // Implementar movimento aleatório
    } else if (caravan->getType() == "Military") {
        std::cout << "Caravana Militar " << caravan->getId()
                  << " movendo-se na ultima direcao conhecida." << std::endl;
        caravan->move("last_direction"); // Implementar lógica de última direção
    }
}

void Simulator::stopCaravanAuto(int caravanId) {
    for (auto& caravan : caravans) {
        if (caravan->getId() == caravanId) {
            caravan->setAuto(false);
            std::cout << "Caravana " << caravanId << " parou o modo automatico." << std::endl;
            return;
        }
    }
    std::cout << "Erro: Caravana com ID " << caravanId << " nao encontrada." << std::endl;
}


void Simulator::createBarbarianCaravan(int l, int c) {
    // Verifica se a posição é válida e desocupada
    if (map.getCell(l, c) == '.') {
        Caravan* barbarian = new BarbarianCaravan(caravans.size() + 1); // ID único
        barbarian->setPosition(l, c);
        caravans.push_back(barbarian); // Adiciona à lista de caravanas
        map.setCell(l, c, '!'); // Marca no mapa com o símbolo de caravana bárbara
        cout << "Caravana barbara criada na posicao (" << l << ", " << c << ")." << endl;
    } else {
        cerr << "Erro: Nao e possível criar uma caravana barbara em (" << l << ", " << c << "). Posiçao invalida ou ocupada." << endl;
    }
}

void Simulator::buyCrewForCaravan(int caravanId, int crewCount) {
    for (auto& caravan : caravans) {
        if (caravan->getId() == caravanId) {
            // Verificar se a caravana está em uma cidade
            if (!caravan->isInCity()) {
                std::cout << "Erro: A caravana " << caravanId << " precisa estar em uma cidade para comprar tripulantes." << std::endl;
                return;
            }

            // Calcular o custo
            int cost = crewCount; // Assumindo 1 moeda por tripulante
            if (wallet.getCoins() < cost) {
                std::cout << "Erro: Moedas insuficientes para comprar " << crewCount << " tripulantes." << std::endl;
                return;
            }

            // Adicionar os tripulantes à caravana
            caravan->setCrew(caravan->getCrew() + crewCount);
            wallet.deductCoins(cost);

            std::cout << "Caravana " << caravanId << " comprou " << crewCount
                      << " tripulantes. Moedas restantes: " << wallet.getCoins() << "." << std::endl;
            return;
        }
    }

    std::cout << "Erro: Caravana com ID " << caravanId << " nao encontrada." << std::endl;
}

void Simulator::removeCaravan(Caravan* caravan) {
    auto it = find(caravans.begin(), caravans.end(), caravan);
    if (it != caravans.end()) {
        map.setCell(caravan->getRow(), caravan->getCol(), '.'); // Limpa a posição no mapa
        delete *it; // Libera memória
        caravans.erase(it);
    }
}


void Simulator::handleTradeCaravanInSandstorm(TradeCaravan* caravan) {
    int chance = rand() % 100;
    bool isOverloaded = (caravan->getCargo() > (caravan->getMaxCargo() / 2));
    int destructionChance = isOverloaded ? 50 : 25;

    if (chance < destructionChance) {
        std::cout << "[Tempestade] Caravana de Comercio ID: " << caravan->getId() << " foi destruida!" << std::endl;
        removeCaravan(caravan); // Implementar lógica de remoção
    } else {
        caravan->setCargo(caravan->getCargo() * 0.75); // Perde 25% da carga
        std::cout << "[Tempestade] Caravana de Comercio ID: " << caravan->getId()
                  << " sobreviveu, mas perdeu 25% da carga." << std::endl;
    }
}

void Simulator::handleMilitaryCaravanInSandstorm(MilitaryCaravan* caravan) {
    caravan->loseCrew(caravan->getCrew() * 0.10); // Perde 10% dos tripulantes

    int chance = rand() % 100;
    if (chance < 33) {
        std::cout << "[Tempestade] Caravana Militar ID: " << caravan->getId() << " foi destruida!" << std::endl;
        removeCaravan(caravan); // Implementar lógica de remoção
    } else {
        std::cout << "[Tempestade] Caravana Militar ID: " << caravan->getId()
                  << " perdeu 10% dos tripulantes, mas sobreviveu." << std::endl;
    }
}
