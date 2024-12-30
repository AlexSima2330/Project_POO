#include "Simulator.h"
#include "BarbarianCaravan.h"
#include <iostream>
#include <algorithm>

using namespace std;

void Simulator::addCaravan(Caravan* caravan, int row, int col) {
    if (map.getCell(row, col) == '.') {
        caravans.push_back(caravan);
        map.setCell(row, col, '0' + caravan->getId());
    } else {
        cerr << "Erro: Posicao (" << row << ", " << col << ") ocupada ou invalida para a Caravana." << endl;
    }
}

bool Simulator::moveCaravan(int caravanId, char direction) {
    for (auto caravan : caravans) {
        if (caravan->getId() == caravanId) {
            if (!caravan->processMovement(map)) {
                std::cout << "Caravana " << caravanId << " nao pode mover-se devido a falta de agua ou tripulantes." << std::endl;
                return false;
            }

            std::string directionStr(1, direction);
            caravan->move(directionStr);

            int newRow = caravan->getRow();
            int newCol = caravan->getCol();
            map.setCell(newRow, newCol, 'C');

            std::cout << "Caravana " << caravanId << " moveu-se para (" << newRow << ", " << newCol << ")." << std::endl;
            return true;
        }
    }
    std::cout << "Caravana com ID " << caravanId << " nao encontrada." << std::endl;
    return false;
}

void Simulator::moveCaravanWithDirection(int caravanId, const std::string& direction) {
    for (auto caravan : caravans) {
        if (caravan->getId() == caravanId && caravan->getType() != "Barbarian") {

            if ((caravan->getType() == "Trade" || caravan->getType() == "Military") && caravan->getCrew() == 0) {
                std::cout << "[Erro] Caravana " << caravanId
                          << " nao pode ser movida manualmente porque esta sem tripulantes." << std::endl;
                return;
            }

            int oldRow = caravan->getRow();
            int oldCol = caravan->getCol();

            if (caravan->isInCity()) {
                City* city = map.getCityAt(oldRow, oldCol);
                if (city) {
                    city->removeCaravan(caravan);
                    caravan->setInCity(false);
                    std::cout << "Caravana " << caravanId << " saiu da cidade " << city->getName() << "." << std::endl;
                }
            }

            if (!caravan->processMovement(map)) {
                std::cout << "[Erro] Caravana " << caravanId << " nao pode mover-se devido a falta de agua." << std::endl;
                return;
            }

            caravan->move(direction);
            auto [newRow, newCol] = map.wrapCoordinates(caravan->getRow(), caravan->getCol());

            char destinationCell = map.getCell(newRow, newCol);
            if (destinationCell == '+') {
                std::cout << "[Erro] Movimento invalido: destino contem um obstaculo." << std::endl;

                caravan->setPosition(oldRow, oldCol);
                return;
            }

            if (std::isdigit(destinationCell)) {
                std::cout << "[Erro] Movimento invalido: destino ja contem outra caravana." << std::endl;

                caravan->setPosition(oldRow, oldCol);
                return;
            }

            if (map.isCity(newRow, newCol)) {
                City* city = map.getCityAt(newRow, newCol);
                if (city) {
                    city->addCaravan(caravan);
                    caravan->setInCity(true);
                    caravan->refillWater();
                    std::cout << "Caravana " << caravanId << " entrou na cidade " << city->getName()
                              << " e foi reabastecida de agua." << std::endl;

                    if (!map.isCity(oldRow, oldCol)) {
                        map.setCell(oldRow, oldCol, '.');
                    }

                    return;
                }
            }

            if (!map.isCity(oldRow, oldCol)) {
                map.setCell(oldRow, oldCol, '.');
            }

            map.setCell(newRow, newCol, '0' + caravanId);
            caravan->status();
            checkCaravanForItem(caravan);
            return;
        }
    }
    std::cout << "[Erro] Caravana com ID " << caravanId << " nao encontrada." << std::endl;
}

void Simulator::buyCaravan(char cityName, char type) {
    City* city = map.getCityByName(cityName);
    if (!city) {
        std::cout << "Erro: Cidade '" << cityName << "' nao encontrada." << std::endl;
        return;
    }

    if (wallet.getCoins() < 100) {
        std::cout << "Erro: Moedas insuficientes para comprar uma caravana." << std::endl;
        return;
    }

    // Percorre as caravanas da cidade
    for (auto& caravan : city->getCaravans()) {
        if ((type == 'C' && caravan->getType() == "Trade") ||
            (type == 'M' && caravan->getType() == "Military") ||
            (type == 'S' && caravan->getType() == "Secret")) {

            if (!caravan->isInCity()) {
                std::cout << "Erro: Esta caravana ja foi comprada nesta cidade." << std::endl;
                return;
            }

            wallet.deductCoins(100);

            caravan->setInCity(true);
            caravan->setAuto(false);
            caravan->setOwned(true);

            this->caravans.push_back(caravan);

            map.setCell(city->getRow(), city->getCol(), '0' + caravan->getId());
            caravan->setPosition(city->getRow(), city->getCol());

            std::cout << "Caravana do tipo '" << type << "' comprada com sucesso na cidade '" << cityName << "'." << std::endl;
            return;
            }
    }

    std::cout << "Erro: Nao há caravanas do tipo '" << type << "' disponiveis para compra na cidade '" << cityName << "'." << std::endl;
}

void Simulator::showCaravanDetails(int caravanId) const {
    for (const auto& caravan : caravans) {
        if (caravan->getId() == caravanId) {
            caravan->status();
            return;
        }
    }
    std::cout << "Caravana com ID " << caravanId << " nao encontrada." << std::endl;
}

void Simulator::buyMerchandise(int caravanId, int quantity) {
    for (auto& caravan : caravans) {
        if (caravan->getId() == caravanId) {
            if (!caravan->isInCity()) {
                cout << "Erro: A caravana " << caravanId << " precisa estar numa cidade para comprar mercadorias." << endl;
                return;
            }

            int cost = quantity;
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
    cout << "Erro: Caravana " << caravanId << " nao encontrada." << endl;
}

void Simulator::sellMerchandise(int caravanId) {
    for (auto& caravan : caravans) {
        if (caravan->getId() == caravanId) {
            if (!caravan->isInCity()) {
                cout << "Erro: A caravana " << caravanId << " precisa estar numa cidade para vender mercadorias." << endl;
                return;
            }

            int revenue = caravan->getCargo() * 2;
            caravan->setCargo(0);
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

            advanceSimulation(1);
            return;
        }
    }
    std::cout << "Erro: Caravana com ID " << caravanId << " nao encontrada." << std::endl;
}

void Simulator::handleTradeCaravanAuto(Caravan* caravan) {
    if (caravan->getType() != "Trade") return;

    int row = caravan->getRow();
    int col = caravan->getCol();
    Item* nearestItem = nullptr;
    Caravan* nearestCaravan = nullptr;
    int minItemDistance = 3;
    int minCaravanDistance = 3;

    for (auto& item : items) {
        int itemRow = item->getRow();
        int itemCol = item->getCol();
        int distance = std::max(std::abs(itemRow - row), std::abs(itemCol - col));

        if (distance <= 2 && distance < minItemDistance) {
            nearestItem = item;
            minItemDistance = distance;
        }
    }

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
                  << " moveu-se para protecao proxima da caravana ID: " << nearestCaravan->getId()
                  << "." << std::endl;
        return;
    }

    std::cout << "[TradeCaravan] ID: " << caravan->getId()
              << " nao encontrou itens ou caravanas proximas." << std::endl;
}

void Simulator::handleMilitaryCaravanAuto(Caravan* caravan) {
    int caravanRow = caravan->getRow();
    int caravanCol = caravan->getCol();

    for (auto& otherCaravan : caravans) {
        if (otherCaravan->getType() == "Barbarian") {
            int barbarianRow = otherCaravan->getRow();
            int barbarianCol = otherCaravan->getCol();

            if (abs(barbarianRow - caravanRow) <= 6 && abs(barbarianCol - caravanCol) <= 6) {

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

    std::cout << "Caravana Militar " << caravan->getId() << " nao encontrou barbaros proximos." << std::endl;
}

void Simulator::handleSecretCaravanAuto(Caravan* caravan) {
    if (caravan->getType() != "Secret") return;

    int oldRow = caravan->getRow();
    int oldCol = caravan->getCol();

    if (caravan->isCurrentlyInvisible()) {
        return;
    }

    if (caravan->getCrew() == 0) {
        caravan->becomeObstacle(map);
        return;
    }

    std::vector<std::string> directions = {"2D", "2E", "2C", "2B"};
    bool moved = false;

    for (int i = 0; i < 3; ++i) {
        std::string randomDirection = directions[rand() % directions.size()];

        moveCaravanWithDirection(caravan->getId(), randomDirection);

        if (caravan->getRow() != oldRow || caravan->getCol() != oldCol) {
            moved = true;
            break;
        } else {
            std::cout << "[ERROR] [SecretCaravan] Falha ao mover-se para direcao: "
                      << randomDirection << std::endl;
        }
    }

    int newRow = caravan->getRow();
    int newCol = caravan->getCol();

    if (moved) {
        map.setCell(oldRow, oldCol, '.');
        map.setCell(newRow, newCol, '0' + caravan->getId());

    } else {
        std::cout << "[ERROR] [SecretCaravan] A posicao nao foi atualizada no mapa." << std::endl;
    }
}

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
                  << " move-se aleatoriamente sem tripulacao." << std::endl;
        caravan->moveRandomly(map); // Movimento aleatorio
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
                  << " move-se na ultima direcao conhecida." << std::endl;
        caravan->moveInLastDirection(map);
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
    auto [wrappedRow, wrappedCol] = map.wrapCoordinates(l, c);

    if (map.getCell(wrappedRow, wrappedCol) == '.') {
        BarbarianCaravan* barbarian = new BarbarianCaravan(caravans.size() + 1);
        barbarian->setPosition(wrappedRow, wrappedCol);
        barbarian->setSpawnTurn(elapsedInstants);
        caravans.push_back(barbarian);
        map.setCell(wrappedRow, wrappedCol, '!');

        std::cout << "[BarbarianCaravan] Criada na posicao (" << wrappedRow << ", " << wrappedCol << ")." << std::endl;
    } else {
        std::cerr << "[Erro] Nao foi possivel criar uma caravana barbara em (" << wrappedRow << ", " << wrappedCol << "). Posicao ocupada ou invalida." << std::endl;
    }
}

void Simulator::buyCrewForCaravan(int caravanId, int crewCount) {
    for (auto& caravan : caravans) {
        if (caravan->getId() == caravanId) {
            if (!caravan->isInCity()) {
                std::cout << "Erro: A caravana " << caravanId << " precisa estar em uma cidade para comprar tripulantes." << std::endl;
                return;
            }

            int cost = crewCount;
            if (wallet.getCoins() < cost) {
                std::cout << "Erro: Moedas insuficientes para comprar " << crewCount << " tripulantes." << std::endl;
                return;
            }

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
        map.setCell(caravan->getRow(), caravan->getCol(), '.');
        delete *it; // Liberta memoria
        caravans.erase(it);
    }
}

void Simulator::handleTradeCaravanInSandstorm(TradeCaravan* caravan) {
    int chance = rand() % 100;
    bool isOverloaded = (caravan->getCargo() > (caravan->getMaxCargo() / 2));
    int destructionChance = isOverloaded ? 50 : 25;

    if (chance < destructionChance) {
        std::cout << "[Tempestade] Caravana de Comercio ID: " << caravan->getId() << " foi destruida!" << std::endl;
        removeCaravan(caravan);
    } else {
        caravan->setCargo(caravan->getCargo() * 0.75); // -25%
        std::cout << "[Tempestade] Caravana de Comercio ID: " << caravan->getId()
                  << " sobreviveu, mas perdeu 25% da carga." << std::endl;
    }
}

void Simulator::handleMilitaryCaravanInSandstorm(MilitaryCaravan* caravan) {
    caravan->loseCrew(caravan->getCrew() * 0.10); // -10%

    int chance = rand() % 100;
    if (chance < 33) {
        std::cout << "[Tempestade] Caravana Militar ID: " << caravan->getId() << " foi destruida!" << std::endl;
        removeCaravan(caravan);
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

    Caravan* target = nullptr;
    for (auto& other : caravans) {
        if (other->getType() != "Barbarian"  && !other->isCurrentlyInvisible()) {
            int targetRow = other->getRow();
            int targetCol = other->getCol();

            if ((targetRow == oldRow && abs(targetCol - oldCol) <= 8) ||
                (targetCol == oldCol && abs(targetRow - oldRow) <= 8)) {
                target = other;
                break;
            }
        }
    }

    if (target) {
        int targetRow = target->getRow();
        int targetCol = target->getCol();

        if (targetRow > oldRow) {
            caravan->move("B");
        } else if (targetRow < oldRow) {
            caravan->move("C");
        }

        if (targetCol > oldCol) {
            caravan->move("D");
        } else if (targetCol < oldCol) {
            caravan->move("E");
        }
    } else {
        std::string directions[] = {"C", "B", "D", "E"};
        int index = rand() % 4;
        caravan->move(directions[index]);
    }

    int newRow = caravan->getRow();
    int newCol = caravan->getCol();

    if (map.getCell(newRow, newCol) == '.') {
        map.setCell(oldRow, oldCol, '.');
        map.setCell(newRow, newCol, '!');
    } else {
        caravan->setPosition(oldRow, oldCol);
    }

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
        map.setCell(row, col, '!');
        std::cout << "Nova caravana barbara apareceu em (" << row << ", " << col << ")." << std::endl;
    }
}