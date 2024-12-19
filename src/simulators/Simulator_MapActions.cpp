#include "Simulator.h"
#include <iostream>
#include <fstream>
#include <cctype>

using namespace std;

bool Simulator::loadMap(const std::string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o ficheiro: " << filename << endl;
        return false;
    }

    int rowsConfig, colsConfig, initialCoins;
    file >> rowsConfig >> colsConfig >> initialCoins;

    mapRows = rowsConfig;
    mapCols = colsConfig;

    map = Map(rowsConfig, colsConfig);
    wallet = Wallet(initialCoins);
    bufferRows = rowsConfig;
    bufferCols = colsConfig;
    buffer = Buffer(bufferRows, bufferCols);

    for (int i = 0; i < rowsConfig; ++i) {
        for (int j = 0; j < colsConfig; ++j) {
            char cell;
            file >> cell;
            if (cell == '.') {
                map.setCell(i, j, cell); // Zona vazia
            } else if (std::islower(cell)) {
                map.addCity(cell, i, j); // Adiciona cidade
            } else if (std::isdigit(cell)) {
                map.setCell(i, j, cell); // Caravanas por ID
            } else {
                map.setCell(i, j, cell); // Outros elementos
            }
        }
    }

    file.close();
    return true;
}


void Simulator::displayMap() {
    buffer.clear();

    cout << "Colunas = " << mapCols << endl;
    cout << "Linhas  = " << mapRows << endl;

    int startRow = (bufferRows > map.getRows()) ? (bufferRows - map.getRows()) / 2 : 0;
    int startCol = (bufferCols > map.getCols()) ? (bufferCols - map.getCols()) / 2 : 0;

    // Preenche o buffer com os elementos do mapa
    for (int i = 0; i < map.getRows(); ++i) {
        for (int j = 0; j < map.getCols(); ++j) {
            buffer.setCursor(startRow + i, startCol + j);
            buffer.putChar(map.getCell(i, j)); // Adiciona o conteúdo real da célula do mapa
        }
    }

    // Exibe o conteúdo do buffer no terminal
    buffer.printToConsole();

    // Exibe moedas iniciais
    cout << "Moedas iniciais: " << wallet.getCoins() << std::endl;
}


// Métodos pedidos da fase 2 ligados ao mapa/ambiente
void Simulator::listCityCaravans(char cityName) const {
    cout << "Listar caravanas da cidade " << cityName << " (não implementado)." << endl;
}

void Simulator::createSandstorm(int l, int c, int r) {
    cout << "Criar tempestade de areia em (" << l << "," << c << ") com raio " << r << " (não implementado)." << endl;
}