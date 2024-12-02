#include "Simulator.h"
#include <iostream>
#include <sstream>

// Construtor
Simulator::Simulator(int mapRows, int mapCols, int bufferRows, int bufferCols)
    : map(mapRows, mapCols), bufferRows(bufferRows), bufferCols(bufferCols) {
    std::cout << "Mapa criado com tamanho " << mapRows << "x" << mapCols << "." << std::endl;
}

// Adicionar caravana ao mapa
void Simulator::addCaravan(Caravan* caravan, int row, int col) {
    if (map.getCell(row, col) == '.') {
        caravans.push_back(caravan);
        map.setCell(row, col, 'C');
        // Mensagem simplificada
        std::cout << "Caravana " << caravan->getId() << " adicionada na posicao (" << row << ", " << col << ")." << std::endl;
    } else {
        std::cerr << "Erro: Posicao (" << row << ", " << col << ") ocupada ou invalida." << std::endl;
    }
}

// Carregar mapa a partir de ficheiro
bool Simulator::loadMap(const std::string& filename) {
    if (map.loadFromFile(filename)) {
        std::cout << "Mapa carregado com sucesso do ficheiro: " << filename << std::endl;
        return true;
    } else {
        std::cerr << "Erro ao carregar o mapa do ficheiro: " << filename << std::endl;
        return false;
    }
}

// Executar a lógica principal do simulador
void Simulator::run() {
    std::cout << "Iniciando a simulacao..." << std::endl;

    while (true) {
        displayMap(); // Mostra o mapa

        // Leitura de comandos
        std::string command;
        std::cout << "Digite um comando (sair, status, move <id> <direcao>, auto <id>): ";
        std::getline(std::cin, command);

        if (command == "sair") {
            std::cout << "Simulacao terminada." << std::endl;
            break;
        } else if (command == "status") {
            showCaravanStatus();
        } else if (command.find("move") == 0) {
            int caravanId;
            char direction;
            std::istringstream iss(command);
            iss >> command >> caravanId >> direction;

            if (!moveCaravan(caravanId, direction)) {
                std::cout << "Movimento invalido." << std::endl;
            }
        } else if (command.find("auto") == 0) {
            // Ativa comportamento automático (a implementar)
        } else {
            std::cout << "Comando invalido." << std::endl;
        }
    }
}


// Exibir o estado atual do mapa
void Simulator::displayMap() {
    std::cout << "Mapa Atual:" << std::endl;
    map.display(); // Mostra apenas o mapa
}

// Mostrar o estado das caravanas
void Simulator::showCaravanStatus() const {
    std::cout << "Estado das Caravanas:" << std::endl;
    for (auto caravan : caravans) {
        caravan->status();
    }
}

// Mover caravanas
bool Simulator::moveCaravan(int caravanId, char direction) {
    for (auto caravan : caravans) {
        if (caravan->getId() == caravanId) {
            if (!caravan->isActive()) {
                std::cout << "Caravana " << caravanId << " esta inativa. Não pode mover-se." << std::endl;
                return false;
            }

            const int waterConsumptionPerMove = 10;
            if (caravan->getWater() < waterConsumptionPerMove) {
                caravan->loseCrew(1);
                std::cout << "Caravana " << caravanId << " perdeu 1 tripulante por falta de agua. Tripulantes restantes: "
                          << caravan->getCrew() << std::endl;
                return false;
            }

            int oldRow = caravan->getRow();
            int oldCol = caravan->getCol();
            int oldCellKey = oldRow * map.getCols() + oldCol; // Gera uma chave única para a célula
            char oldCellContent = map.getCell(oldRow, oldCol); // Conteúdo original da célula antiga

            caravan->consumeWater(waterConsumptionPerMove);
            caravan->move(direction);

            int newRow = caravan->getRow();
            int newCol = caravan->getCol();
            int newCellKey = newRow * map.getCols() + newCol; // Gera uma chave única para a nova célula

            if (newRow >= 0 && newRow < map.getRows() &&
                newCol >= 0 && newCol < map.getCols()) {

                // Se a célula contém um carregador, reabastece
                if (map.getCell(newRow, newCol) == 'c') {
                    caravan->refillWater();
                    std::cout << "Caravana " << caravanId << " reabasteceu agua no carregador em (" << newRow << ", " << newCol << ")."
                              << std::endl;
                }

                // Move a caravana e restaura o estado do mapa
                if (map.getCell(newRow, newCol) == '.' || map.getCell(newRow, newCol) == 'c') {
                    // Restaura o conteúdo original da célula antiga
                    if (originalCellContent.count(oldCellKey)) {
                        map.setCell(oldRow, oldCol, originalCellContent[oldCellKey]);
                        originalCellContent.erase(oldCellKey);
                    } else {
                        map.setCell(oldRow, oldCol, '.');
                    }

                    // Guarda o estado original da nova célula se ainda não estiver salvo
                    if (map.getCell(newRow, newCol) == 'c') {
                        originalCellContent[newCellKey] = 'c';
                    }

                    // Atualiza a nova célula com a caravana
                    map.setCell(newRow, newCol, 'C');
                    return true;
                } else {
                    caravan->setPosition(oldRow, oldCol); // Reverte movimento
                    std::cout << "Movimento invalido para a caravana " << caravanId << "." << std::endl;
                    return false;
                }
            } else {
                caravan->setPosition(oldRow, oldCol); // Reverte movimento
                std::cout << "Movimento fora dos limites para a caravana " << caravanId << "." << std::endl;
                return false;
            }
        }
    }
    std::cout << "Caravana com ID " << caravanId << " não encontrada." << std::endl;
    return false;
}