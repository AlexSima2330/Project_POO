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
    file >> rowsConfig >> colsConfig >> initialCoins >> timeBetweenItems >> itemDuration >> maxItems >> timeBetweenBarbarians >> barbarianDuration;;

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
                map.setCell(i, j, cell);
            } else if (std::islower(cell)) {
                map.addCity(cell, i, j);
            } else if (std::isdigit(cell)) {
                map.setCell(i, j, cell);
            } else {
                map.setCell(i, j, cell);
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

    for (const auto& caravan : caravans) {
        if (caravan->getType() == "Secret" && caravan->isCurrentlyInvisible()) {
            map.setCell(caravan->getRow(), caravan->getCol(), '.');
        }
    }

    for (const auto& caravan : caravans) {
        if (caravan->getType() == "Secret" && caravan->isCurrentlyInvisible()) {
            continue;
        }

        int row = caravan->getRow();
        int col = caravan->getCol();

        if (!map.isCity(row, col)) {
            buffer.setCursor(startRow + row, startCol + col);
            buffer.putChar('0' + caravan->getId());
        }
    }

    for (int i = 0; i < map.getRows(); ++i) {
        for (int j = 0; j < map.getCols(); ++j) {
            buffer.setCursor(startRow + i, startCol + j);
            buffer.putChar(map.getCell(i, j));
        }
    }

    for (const auto& item : items) {
        buffer.setCursor(startRow + item->getRow(), startCol + item->getCol());
        buffer.putChar('I');
    }

    for (const auto& caravan : caravans) {
        if (caravan->getType() == "Secret" && caravan->isCurrentlyInvisible()) {
            map.setCell(caravan->getRow(), caravan->getCol(), '0' + caravan->getId());
        }
    }

    buffer.printToConsole();

    cout << "moedas " << wallet.getCoins() << endl;
    cout << "instantes_entre_novos_itens " << timeBetweenItems << endl;
    cout << "duracao_item " << itemDuration << endl;
    cout << "max_itens " << maxItems << endl;
    cout << "instantes_entre_novos_barbaros " << timeBetweenBarbarians << std::endl;
    cout << "duracao_barbaros " << barbarianDuration << std::endl;
}

void Simulator::listCityCaravans(char cityName) const {
    City* city = map.getCityByName(cityName);
    if (city) {
        std::cout << "Listando caravanas na cidade " << cityName << ":\n";
        city->listCaravans();
    } else {
        std::cout << "Erro: Cidade '" << cityName << "' nao encontrada." << std::endl;
    }
}

void Simulator::createSandstorm(int l, int c, int r) {
    cout << "Criando tempestade de areia no centro (" << l << ", " << c << ") com raio " << r << "." << endl;

    for (int i = l - r; i <= l + r; ++i) {
        for (int j = c - r; j <= c + r; ++j) {
            auto [wrappedRow, wrappedCol] = map.wrapCoordinates(i, j);

            char cellContent = map.getCell(wrappedRow, wrappedCol);

            if (cellContent == 'C') {
                cout << "Caravana afetada pela tempestade em (" << wrappedRow << ", " << wrappedCol << ")." << endl;
                for (auto caravan : caravans) {
                    if (caravan->getRow() == wrappedRow && caravan->getCol() == wrappedCol) {
                        caravan->loseCrew(10);
                        if (!caravan->isActive()) {
                            caravan->becomeObstacle(map);
                        }
                    }
                }
            } else if (cellContent == '!') {
                cout << "Caravana barbara afetada pela tempestade em (" << wrappedRow << ", " << wrappedCol << ")." << endl;
                for (auto caravan : caravans) {
                    if (caravan->getRow() == wrappedRow && caravan->getCol() == wrappedCol) {
                        caravan->loseCrew(10);
                        if (!caravan->isActive()) {
                            caravan->becomeObstacle(map);
                        }
                    }
                }
            } else {
                cout << "Nenhum elemento afetado em (" << wrappedRow << ", " << wrappedCol << ")." << endl;
            }
        }
    }
}