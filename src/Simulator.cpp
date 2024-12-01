#include "Simulator.h"
#include <iostream>
#include <sstream>

// Construtor
Simulator::Simulator(int mapRows, int mapCols, int bufferRows, int bufferCols)
    : map(mapRows, mapCols), bufferRows(bufferRows), bufferCols(bufferCols) {
    std::cout << "Simulator inicializado com mapa de tamanho " << mapRows << "x" << mapCols << std::endl;
}

// Adicionar caravana ao mapa
void Simulator::addCaravan(Caravan* caravan, int row, int col) {
    try {
        std::cout << "Tentando adicionar caravana na posicao (" << row << ", " << col << ")..." << std::endl;

        if (map.getCell(row, col) == '.') { // Verifica se a posição está livre
            caravans.push_back(caravan);  // Adiciona a caravana à lista
            map.setCell(row, col, 'C');   // Marca a posição no mapa
            std::cout << "Caravana adicionada na posicao (" << row << ", " << col << ")." << std::endl;
        } else {
            std::cerr << "Erro: A posicao (" << row << ", " << col << ") esta ocupada ou e invalida." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro ao adicionar caravana: " << e.what() << std::endl;
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
        displayMap(); // Exibe o mapa

        // Adiciona lógica de interação (ex.: comandos do utilizador)
        std::string command;
        std::cout << "Digite um comando (sair, status, move <id> <direcao>): ";
        std::getline(std::cin, command);

        if (command == "sair") {
            std::cout << "Simulacao terminada." << std::endl;
            break;
        } else if (command == "status") {
            showCaravanStatus();
        } else if (command.find("move") == 0) {
            // Exemplo de comando: "move 1 D"
            int caravanId;
            char direction;
            std::istringstream iss(command);
            iss >> command >> caravanId >> direction;

            if (!moveCaravan(caravanId, direction)) {
                std::cout << "Falha ao mover a caravana." << std::endl;
            }
        } else {
            std::cout << "Comando invalido!" << std::endl;
        }
    }
}

// Exibir o estado atual do mapa
void Simulator::displayMap() {
    std::cout << "Mapa Atual:" << std::endl;
    map.display();
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
            int oldRow = caravan->getRow();
            int oldCol = caravan->getCol();

            caravan->move(direction);

            int newRow = caravan->getRow();
            int newCol = caravan->getCol();

            if (newRow >= 0 && newRow < map.getRows() &&
                newCol >= 0 && newCol < map.getCols()) {

                // Verifica se a célula de destino contém um recurso
                if (map.getCell(newRow, newCol) == 'a') {
                    std::cout << "Caravana " << caravanId << " coletou um recurso na posicao (" << newRow << ", " << newCol << ")." << std::endl;
                    caravan->addResource(); // Coleta o recurso
                    map.setCell(newRow, newCol, '.'); // Remove o recurso do mapa
                }

                // Verifica se a célula está livre para movimentação
                if (map.getCell(newRow, newCol) == '.') {
                    map.setCell(oldRow, oldCol, '.'); // Limpa a posição antiga
                    map.setCell(newRow, newCol, 'C'); // Atualiza nova posição
                    return true;
                } else {
                    caravan->setPosition(oldRow, oldCol); // Reverte movimento
                    std::cout << "Movimento inválido para a caravana " << caravanId << "." << std::endl;
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

