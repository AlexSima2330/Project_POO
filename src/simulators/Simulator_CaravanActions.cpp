#include "Simulator.h"
#include "BarbarianCaravan.h"
#include <iostream>
#include <algorithm>

#include "Fight.h"

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
        if (caravan->getId() == caravanId && caravan->getType() != "Barbarian") {

            if ((caravan->getType() == "Trade" || caravan->getType() == "Military") && caravan->getCrew() == 0) {
                std::cout << "[Erro] Caravana " << caravanId
                          << " não pode ser movida manualmente porque está sem tripulantes." << std::endl;
                return;
            }

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
                std::cout << "[Erro] Movimento invalido: destino contem um obstaculo." << std::endl;

                // Restaura a posição anterior da caravana
                caravan->setPosition(oldRow, oldCol);
                return;
            }

            // Verifica se o destino já contém uma caravana
            if (std::isdigit(destinationCell)) {
                std::cout << "[Erro] Movimento invalido: destino ja contem outra caravana." << std::endl;

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
            checkCaravanForItem(caravan);
            return;
        }
    }
    std::cout << "[Erro] Caravana com ID " << caravanId << " não encontrada." << std::endl;
}

// Novos métodos relacionados a caravanas
void Simulator::buyCaravan(char cityName, char type) {
    City* city = map.getCityByName(cityName);
    if (!city) {
        std::cout << "Erro: Cidade '" << cityName << "' não encontrada." << std::endl;
        return;
    }

    // Verifica se há moedas suficientes
    if (wallet.getCoins() < 100) {
        std::cout << "Erro: Moedas insuficientes para comprar uma caravana." << std::endl;
        return;
    }

    // Percorre as caravanas da cidade
    for (auto& caravan : city->getCaravans()) {
        if ((type == 'C' && caravan->getType() == "Trade") ||
            (type == 'M' && caravan->getType() == "Military") ||
            (type == 'S' && caravan->getType() == "Secret")) {

            // Verifica se a caravana já foi comprada
            if (!caravan->isInCity()) {
                std::cout << "Erro: Esta caravana já foi comprada nesta cidade." << std::endl;
                return;
            }

            // Deduz moedas
            wallet.deductCoins(100);

            // Marca a caravana como comprada
            caravan->setInCity(true); // Continua na cidade
            caravan->setAuto(false); // Desativa o modo automático
            caravan->setOwned(true); // Nova flag para indicar que foi comprada

            // Adiciona ao vetor global de caravanas
            this->caravans.push_back(caravan);

            // Atualiza o mapa com o ID da caravana
            map.setCell(city->getRow(), city->getCol(), '0' + caravan->getId());
            caravan->setPosition(city->getRow(), city->getCol());

            std::cout << "Caravana do tipo '" << type << "' comprada com sucesso na cidade '" << cityName << "'." << std::endl;
            return;
            }
    }

    std::cout << "Erro: Não há caravanas do tipo '" << type << "' disponíveis para compra na cidade '" << cityName << "'." << std::endl;
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
            std::cout << "Caravana " << caravanId << " está agora em modo automático." << std::endl;

            // Processa imediatamente o movimento automático
            advanceSimulation(1);
            return;
        }
    }
    std::cout << "Erro: Caravana com ID " << caravanId << " não encontrada." << std::endl;
}


void Simulator::handleTradeCaravanAuto(Caravan* caravan) {
    if (caravan->getType() != "Trade") return;

    int row = caravan->getRow();
    int col = caravan->getCol();
    Item* nearestItem = nullptr;
    Caravan* nearestCaravan = nullptr;
    int minItemDistance = 3;
    int minCaravanDistance = 3;

    // 1️⃣ Procurar itens próximos
    for (auto& item : items) {
        int itemRow = item->getRow();
        int itemCol = item->getCol();
        int distance = std::max(std::abs(itemRow - row), std::abs(itemCol - col));

        if (distance <= 2 && distance < minItemDistance) {
            nearestItem = item;
            minItemDistance = distance;
        }
    }

    // 2️⃣ Mover em direção ao item mais próximo
    if (nearestItem) {
        int targetRow = nearestItem->getRow();
        int targetCol = nearestItem->getCol();

        if (targetRow > row) {
            moveCaravanWithDirection(caravan->getId(), "B");
        } else if (targetRow < row) {
            moveCaravanWithDirection(caravan->getId(), "C");
        }

        if (targetCol > col) {
            moveCaravanWithDirection(caravan->getId(), "D");
        } else if (targetCol < col) {
            moveCaravanWithDirection(caravan->getId(), "E");
        }

        std::cout << "[TradeCaravan] ID: " << caravan->getId()
                  << " moveu-se para apanhar item em (" << targetRow << ", " << targetCol << ")." << std::endl;
        return;
    }

    // 3️⃣ Procurar outra caravana amiga próxima
    for (auto& otherCaravan : caravans ) {
        if (otherCaravan != caravan && otherCaravan->isOwned() && !otherCaravan->isCurrentlyInvisible()) {
            int otherRow = otherCaravan->getRow();
            int otherCol = otherCaravan->getCol();
            int distance = std::max(std::abs(otherRow - row), std::abs(otherCol - col));

            if (distance <= 2 && distance < minCaravanDistance) {
                nearestCaravan = otherCaravan;
                minCaravanDistance = distance;
            }
        }
    }

    // 4️⃣ Mover em direção à caravana amiga mais próxima
    if (nearestCaravan) {
        int targetRow = nearestCaravan->getRow();
        int targetCol = nearestCaravan->getCol();

        if (targetRow > row) {
            moveCaravanWithDirection(caravan->getId(), "B");
        } else if (targetRow < row) {
            moveCaravanWithDirection(caravan->getId(), "C");
        }

        if (targetCol > col) {
            moveCaravanWithDirection(caravan->getId(), "D");
        } else if (targetCol < col) {
            moveCaravanWithDirection(caravan->getId(), "E");
        }

        std::cout << "[TradeCaravan] ID: " << caravan->getId()
                  << " moveu-se para proteção próxima da caravana ID: " << nearestCaravan->getId()
                  << "." << std::endl;
        return;
    }

    std::cout << "[TradeCaravan] ID: " << caravan->getId()
              << " não encontrou itens ou caravanas próximas." << std::endl;
}

void Simulator::handleMilitaryCaravanAuto(Caravan* caravan) {
    int caravanRow = caravan->getRow();
    int caravanCol = caravan->getCol();

    for (auto& otherCaravan : caravans) {
        if (otherCaravan->getType() == "Barbarian") {
            int barbarianRow = otherCaravan->getRow();
            int barbarianCol = otherCaravan->getCol();

            // Verifica se está dentro de 6 posições
            if (abs(barbarianRow - caravanRow) <= 6 && abs(barbarianCol - caravanCol) <= 6) {
                std::cout << "[DEBUG] Caravana Militar " << caravan->getId()
                          << " encontrou um bárbaro na posição (" << barbarianRow << ", " << barbarianCol << ")."
                          << std::endl;

                // Movimento validado usando moveCaravanWithDirection
                if (barbarianRow > caravanRow) {
                    moveCaravanWithDirection(caravan->getId(), "B");
                } else if (barbarianRow < caravanRow) {
                    moveCaravanWithDirection(caravan->getId(), "C");
                }

                if (barbarianCol > caravanCol) {
                    moveCaravanWithDirection(caravan->getId(), "D");
                } else if (barbarianCol < caravanCol) {
                    moveCaravanWithDirection(caravan->getId(), "E");
                }

                return;
            }
        }
    }

    std::cout << "Caravana Militar " << caravan->getId() << " não encontrou bárbaros próximos." << std::endl;
}

void Simulator::handleSecretCaravanAuto(Caravan* caravan) {
    if (caravan->getType() != "Secret") return;

    // Armazena a posição inicial
    int oldRow = caravan->getRow();
    int oldCol = caravan->getCol();

    std::cout << "[DEBUG] [SecretCaravan] ID: " << caravan->getId()
              << " na posição inicial (" << oldRow << ", " << oldCol << ") iniciando movimento automático." << std::endl;

    // 1️⃣ Invisível → Não se move
    if (caravan->isCurrentlyInvisible()) {
        std::cout << "[DEBUG] [SecretCaravan] ID: " << caravan->getId()
                  << " está invisível e não se move neste turno." << std::endl;
        return;
    }

    // 2️⃣ Sem Tripulantes → Torna-se obstáculo
    if (caravan->getCrew() == 0) {
        caravan->becomeObstacle(map);
        std::cout << "[DEBUG] [SecretCaravan] ID: " << caravan->getId()
                  << " ficou sem tripulantes e tornou-se um obstáculo permanente no mapa." << std::endl;
        return;
    }

    // 3️⃣ Movimento Aleatório com Movimento Especial
    std::vector<std::string> directions = {"2D", "2E", "2C", "2B"};
    bool moved = false;

    for (int i = 0; i < 3; ++i) {
        std::string randomDirection = directions[rand() % directions.size()];
        std::cout << "[DEBUG] [SecretCaravan] Tentativa " << (i + 1)
                  << ": direção " << randomDirection << std::endl;

        moveCaravanWithDirection(caravan->getId(), randomDirection);

        // Comparar com a posição inicial (oldRow e oldCol)
        if (caravan->getRow() != oldRow || caravan->getCol() != oldCol) {
            std::cout << "[DEBUG] [SecretCaravan] Movimento bem-sucedido para ("
                      << caravan->getRow() << ", " << caravan->getCol() << ") na direção: "
                      << randomDirection << std::endl;
            moved = true;
            break;
        } else {
            std::cout << "[ERROR] [SecretCaravan] Falha ao mover-se para direção: "
                      << randomDirection << std::endl;
        }
    }

    // 4️⃣ Atualizar Mapa após Movimento
    int newRow = caravan->getRow();
    int newCol = caravan->getCol();

    if (moved) {
        map.setCell(oldRow, oldCol, '.'); // Limpa a posição antiga
        map.setCell(newRow, newCol, '0' + caravan->getId()); // Atualiza a nova posição

        std::cout << "[DEBUG] [SecretCaravan] Mapa atualizado com nova posição ("
                  << newRow << ", " << newCol << ")." << std::endl;
    } else {
        std::cout << "[ERROR] [SecretCaravan] A posição não foi atualizada no mapa." << std::endl;
    }
}



// Simulator_CaravanActions.cpp
void Simulator::handleCaravanWithoutCrew(Caravan* caravan) {
    caravan->incrementAutoTurnsWithoutCrew();

    if (caravan->getType() == "Trade") {
        if (caravan->getAutoTurnsWithoutCrew() >= 5) {
            std::cout << "[TradeCaravan] ID: " << caravan->getId()
                      << " desapareceu após 5 turnos sem tripulantes." << std::endl;
            removeCaravan(caravan);
            return;
        }
        std::cout << "[TradeCaravan] ID: " << caravan->getId()
                  << " move-se aleatoriamente sem tripulação." << std::endl;
        caravan->moveRandomly(map); // Movimento aleatório garantido
        return;
    }

    if (caravan->getType() == "Military") {
        if (caravan->getAutoTurnsWithoutCrew() >= 7) {
            std::cout << "[MilitaryCaravan] ID: " << caravan->getId()
                      << " desapareceu após 7 turnos sem tripulantes." << std::endl;
            removeCaravan(caravan);
            return;
        }
        std::cout << "[MilitaryCaravan] ID: " << caravan->getId()
                  << " move-se na última direção conhecida." << std::endl;
        caravan->moveInLastDirection(map);
        return;
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
    std::cout << "Erro: Caravana com ID " << caravanId << " não encontrada." << std::endl;
}


void Simulator::createBarbarianCaravan(int l, int c) {
    auto [wrappedRow, wrappedCol] = map.wrapCoordinates(l, c);

    // Verifica se a célula está livre para criar a caravana
    if (map.getCell(wrappedRow, wrappedCol) == '.') {
        BarbarianCaravan* barbarian = new BarbarianCaravan(caravans.size() + 1);
        barbarian->setPosition(wrappedRow, wrappedCol);
        barbarian->setSpawnTurn(elapsedInstants);
        caravans.push_back(barbarian);
        map.setCell(wrappedRow, wrappedCol, '!'); // Representação visual no mapa

        std::cout << "[BarbarianCaravan] Criada na posicao (" << wrappedRow << ", " << wrappedCol << ")." << std::endl;
    } else {
        std::cerr << "[Erro] Não foi possivel criar uma caravana barbara em (" << wrappedRow << ", " << wrappedCol << "). Posicao ocupada ou invalida." << std::endl;
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

    std::cout << "Erro: Caravana com ID " << caravanId << " não encontrada." << std::endl;
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
        std::cout << "[Tempestade] Caravana de Comercio ID: " << caravan->getId() << " foi destruída!" << std::endl;
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
        std::cout << "[Tempestade] Caravana Militar ID: " << caravan->getId() << " foi destruída!" << std::endl;
        removeCaravan(caravan); // Implementar lógica de remoção
    } else {
        std::cout << "[Tempestade] Caravana Militar ID: " << caravan->getId()
                  << " perdeu 10% dos tripulantes, mas sobreviveu." << std::endl;
    }
}

void Simulator::handleCombatResult(bool playerWon) {
    if (playerWon) {
        totalCombatsWon++;
    }
}

void Simulator::handleBarbarianCaravanAuto(BarbarianCaravan* caravan) {
    if (!caravan->isActive()) {
        removeCaravan(caravan);
        return;
    }

    int oldRow = caravan->getRow();
    int oldCol = caravan->getCol();

    // 🎯 **1. Verificar se há uma caravana na mesma linha ou coluna dentro do raio de 8 posições**
    Caravan* target = nullptr;
    for (auto& other : caravans) {
        if (other->getType() != "Barbarian"  && !other->isCurrentlyInvisible()) {
            int targetRow = other->getRow();
            int targetCol = other->getCol();

            // Verifica se está na mesma linha ou coluna e dentro do alcance de 8 posições
            if ((targetRow == oldRow && abs(targetCol - oldCol) <= 8) ||
                (targetCol == oldCol && abs(targetRow - oldRow) <= 8)) {
                target = other;
                break;
            }
        }
    }

    if (target) {
        // **Movimento Direcionado**
        int targetRow = target->getRow();
        int targetCol = target->getCol();

        if (targetRow > oldRow) {
            caravan->move("B"); // Baixo
        } else if (targetRow < oldRow) {
            caravan->move("C"); // Cima
        }

        if (targetCol > oldCol) {
            caravan->move("D"); // Direita
        } else if (targetCol < oldCol) {
            caravan->move("E"); // Esquerda
        }
    } else {
        // **Movimento Aleatório**
        std::string directions[] = {"C", "B", "D", "E"};
        int index = rand() % 4;
        caravan->move(directions[index]);
    }

    // 🔄 **Atualiza a Nova Posição no Mapa**
    int newRow = caravan->getRow();
    int newCol = caravan->getCol();

    if (map.getCell(newRow, newCol) == '.') {
        map.setCell(oldRow, oldCol, '.'); // Limpa a posição antiga
        map.setCell(newRow, newCol, '!'); // Atualiza para a nova posição
    } else {
        // Se a célula estiver ocupada, restaura a posição anterior
        caravan->setPosition(oldRow, oldCol);
    }

    // 🕒 **Desaparecimento após `barbarianDuration` turnos**
    if (elapsedInstants - caravan->getSpawnTurn() >= barbarianDuration) {
        removeCaravan(caravan);
        std::cout << "[BarbarianCaravan] " << caravan->getId()
                  << " desapareceu apos " << barbarianDuration << " turnos." << std::endl;
    }
}

void Simulator::spawnBarbarianCaravan() {
    int row = rand() % map.getRows();
    int col = rand() % map.getCols();

    if (map.getCell(row, col) == '.') {
        BarbarianCaravan* barbarian = new BarbarianCaravan(caravans.size() + 1);
        barbarian->setPosition(row, col);
        barbarian->setSpawnTurn(elapsedInstants);
        caravans.push_back(barbarian);
        map.setCell(row, col, '!'); // Representação visual
        std::cout << "Nova caravana bárbara apareceu em (" << row << ", " << col << ")." << std::endl;
    }
}