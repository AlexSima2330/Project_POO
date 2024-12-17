#include "Simulator.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <Wallet.h>
#include <Map.h>
#include <Caravan.h>
using namespace std;

// Construtor
Simulator::Simulator() : map(0, 0), bufferRows(0), bufferCols(0), buffer(0, 0), wallet(0) {
    cout << "Mapa criado com sucesso!" << endl;
}

// Adicionar caravana ao mapa
void Simulator::addCaravan(Caravan* caravan, int row, int col) {
    if (map.getCell(row, col) == '.') {
        caravans.push_back(caravan);
        map.setCell(row, col, 'C');
    } else {
        cerr << "Erro: Posicao (" << row << ", " << col << ") ocupada ou invalida para a Caravana." << endl;
    }
}

// Carregar mapa a partir do ficheiro
bool Simulator::loadMap(const std::string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o ficheiro: " << filename << endl << "A sair..." << endl;
        return false;
    }

    int rowsConfig, colsConfig, initialCoins;
    file >> rowsConfig >> colsConfig >> initialCoins;

    mapRows = rowsConfig;
    mapCols = colsConfig;

    map = Map(rowsConfig, colsConfig);
    wallet = Wallet(initialCoins);
    bufferRows = 10;
    bufferCols = 20;
    buffer = Buffer(bufferRows, bufferCols);

    for (int i = 0; i < rowsConfig; ++i) {
        for (int j = 0; j < colsConfig; ++j) {
            char cell;
            file >> cell;
            map.setCell(i, j, cell);
            if (islower(cell)) {
                map.addCity(cell, i, j);
            }
        }
    }

    file.close();
    return true;
}



// Executar a lógica principal do simulador
void Simulator::run() {
    while (true) {
        displayMap(); // Mostra o mapa

        // Leitura de comandos
        string command;
        cout << "Digite um comando (sair, status, move <id> <direcao>, auto <id>): ";
        getline(cin, command);

        if (command == "sair") {
            cout << "Simulacao terminada." << endl;
            break;
        } else if (command == "status") {
            showCaravanStatus();
        } else if (command.find("move") == 0) {
            int caravanId;
            char direction;
            istringstream iss(command);
            iss >> command >> caravanId >> direction;

            if (!moveCaravan(caravanId, direction)) {
                cout << "Movimento invalido." << endl;
            }
        } else if (command.find("auto") == 0) {
            // Ativa comportamento automático (a implementar)
        } else {
            cout << "Comando invalido." << endl;
        }
    }
}

// Exibir o estado atual do mapa
void Simulator::displayMap() {
    buffer.clear();

    cout << "Colunas = " << mapCols << endl;
    cout << "Linhas  = " << mapRows << endl;

    // Calcular o ponto inicial para desenhar o mapa no buffer
    int startRow = (bufferRows > map.getRows()) ? (bufferRows - map.getRows()) / 2 : 0;
    int startCol = (bufferCols > map.getCols()) ? (bufferCols - map.getCols()) / 2 : 0;

    // Iterar sobre o mapa e desenhá-lo no buffer
    for (int i = 0; i < map.getRows(); ++i) {
        for (int j = 0; j < map.getCols(); ++j) {
            buffer.setCursor(startRow + i, startCol + j);
            buffer.putChar(map.getCell(i, j));
        }
    }

    buffer.printToConsole(); // Desenha o buffer no terminal
    // Depois de buffer.printToConsole();
    cout << "Moedas iniciais: " << wallet.getCoins() << std::endl;
}

// Mostrar o estado das caravanas
void Simulator::showCaravanStatus() const {
    cout << "Estado das Caravanas:" << endl;
    for (auto caravan : caravans) {
        caravan->status();
    }
}

// Mover caravanas
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
            return true;
        }
    }

    buffer << "Caravana com ID " << caravanId << " nao encontrada.\n";
    return false;
}