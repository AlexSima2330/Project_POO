#include "Simulator.h"
#include <iostream>
#include <fstream>
#include <cctype>

using namespace std;

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
            if (std::islower((unsigned char)cell)) {
                map.addCity(cell, i, j);
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

    for (int i = 0; i < map.getRows(); ++i) {
        for (int j = 0; j < map.getCols(); ++j) {
            buffer.setCursor(startRow + i, startCol + j);
            buffer.putChar(map.getCell(i, j));
        }
    }

    buffer.printToConsole();
    cout << "Moedas iniciais: " << wallet.getCoins() << std::endl;
}

// Métodos pedidos da fase 2 ligados ao mapa/ambiente
void Simulator::listCityCaravans(char cityName) const {
    cout << "Listar caravanas da cidade " << cityName << " (não implementado)." << endl;
}

void Simulator::createSandstorm(int l, int c, int r) {
    cout << "Criar tempestade de areia em (" << l << "," << c << ") com raio " << r << " (não implementado)." << endl;
}