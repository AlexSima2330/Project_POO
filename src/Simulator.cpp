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
                //std::cout << "Movimento invalido." << std::endl;
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
            // Processa o consumo de água e verifica inatividade
            const int waterConsumptionPerMove = 10;
            if (!caravan->processMovement(map, waterConsumptionPerMove)) {
                std::cout << "Caravana " << caravanId << " ficou inativa e tornou-se um obstáculo." << std::endl;
                return false;
            }

            // Coordenadas atuais
            int oldRow = caravan->getRow();
            int oldCol = caravan->getCol();

            // Move a caravana
            caravan->move(direction);

            // Ajusta as coordenadas com comportamento circular
            auto [newRow, newCol] = map.wrapCoordinates(caravan->getRow(), caravan->getCol());

            // Verifica se a célula de destino é um obstáculo
            char cellContent = map.getCell(newRow, newCol);
            if (cellContent == '+' || cellContent == 'b' || cellContent == 'a') {
                // Reverte o movimento para a posição original
                caravan->setPosition(oldRow, oldCol);
                std::cout << "[Caravana] ID: " << caravanId << " nao pode ultrapassar obstáculos. Movimento inválido." << std::endl;
                std::cout << "Movimento invalido. A caravana perdeu tempo e recursos." << std::endl;
                return false;
            }

            // Atualiza o mapa
            auto [wrappedOldRow, wrappedOldCol] = map.wrapCoordinates(oldRow, oldCol); // Coordenadas ajustadas para a célula antiga
            map.setCell(wrappedOldRow, wrappedOldCol, '.'); // Limpa a célula antiga
            map.setCell(newRow, newCol, 'C'); // Define a nova célula
            caravan->setPosition(newRow, newCol); // Atualiza a posição na caravana

            // Verifica se a célula contém um carregador
            if (cellContent == 'c') {
                caravan->refillWater();
                std::cout << "Caravana " << caravanId << " reabasteceu agua no carregador em (" << newRow << ", " << newCol << ")." << std::endl;
            }

            // Mensagem de sucesso
            std::cout << "Caravana " << caravanId << " moveu-se para (" << newRow << ", " << newCol << ")." << std::endl;
            return true;
        }
    }

    // Caso o ID da caravana não seja encontrado
    std::cout << "Caravana com ID " << caravanId << " nao encontrada." << std::endl;
    return false;
}






