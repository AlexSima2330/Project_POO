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

    // Remove temporariamente as caravanas secretas invisíveis do mapa
    for (const auto& caravan : caravans) {
        if (caravan->getType() == "Secret" && caravan->isCurrentlyInvisible()) {
            map.setCell(caravan->getRow(), caravan->getCol(), '.'); // Remove temporariamente do mapa
        }
    }

    // Preenche o buffer com as caravanas, respeitando cidades e invisibilidade
    for (const auto& caravan : caravans) {
        if (caravan->getType() == "Secret" && caravan->isCurrentlyInvisible()) {
            continue; // Não exibe caravanas invisíveis
        }

        int row = caravan->getRow();
        int col = caravan->getCol();

        // Certifica-te de que não sobrescreves cidades
        if (!map.isCity(row, col)) {
            buffer.setCursor(startRow + row, startCol + col);
            buffer.putChar('0' + caravan->getId());
        }
    }

    // Preenche o buffer com os elementos do mapa
    for (int i = 0; i < map.getRows(); ++i) {
        for (int j = 0; j < map.getCols(); ++j) {
            buffer.setCursor(startRow + i, startCol + j);
            buffer.putChar(map.getCell(i, j)); // Adiciona o conteúdo real da célula do mapa
        }
    }

    for (const auto& caravan : caravans) {
        if (caravan->getType() == "Secret" && caravan->isCurrentlyInvisible()) {
            map.setCell(caravan->getRow(), caravan->getCol(), '0' + caravan->getId()); // Restaura no mapa
        }
    }

    // Exibe o conteúdo do buffer no terminal
    buffer.printToConsole();

    // Exibe moedas iniciais
    cout << "moedas " << wallet.getCoins() << std::endl;
   /* cout << "instantes_entre_novos_itens  " << wallet.getCoins() << std::endl;
    cout << "duração_item " << wallet.getCoins() << std::endl;
    cout << "max_itens " << wallet.getCoins() << std::endl;
    cout << "preço_venda_mercadoria " << wallet.getCoins() << std::endl;
    cout << "preço_compra_mercadoria " << wallet.getCoins() << std::endl;
    cout << "preço_caravana " << wallet.getCoins() << std::endl;
    cout << "instantes_entre_novos_barbaros " << wallet.getCoins() << std::endl;
    cout << "duração_barbaros " << wallet.getCoins() << std::endl;*/
}


// Métodos pedidos da fase 2 ligados ao mapa/ambiente
void Simulator::listCityCaravans(char cityName) const {
    City* city = map.getCityByName(cityName);
    if (city) {
        std::cout << "Listando caravanas na cidade " << cityName << ":\n";
        city->listCaravans();
    } else {
        std::cout << "Erro: Cidade '" << cityName << "' não encontrada." << std::endl;
    }
}

void Simulator::createSandstorm(int l, int c, int r) {
    cout << "Criando tempestade de areia no centro (" << l << ", " << c << ") com raio " << r << "." << endl;

    // Percorre o quadrado definido pelo raio
    for (int i = l - r; i <= l + r; ++i) {
        for (int j = c - r; j <= c + r; ++j) {
            // Ajusta as coordenadas para o comportamento circular
            auto [wrappedRow, wrappedCol] = map.wrapCoordinates(i, j);

            // Verifica o que existe na célula
            char cellContent = map.getCell(wrappedRow, wrappedCol);

            if (cellContent == 'C') { // Caravana do usuário
                cout << "Caravana afetada pela tempestade em (" << wrappedRow << ", " << wrappedCol << ")." << endl;
                for (auto caravan : caravans) {
                    if (caravan->getRow() == wrappedRow && caravan->getCol() == wrappedCol) {
                        caravan->loseCrew(10); // Reduz tripulantes
                        if (!caravan->isActive()) {
                            caravan->becomeObstacle(map); // Torna-se obstáculo se inativa
                        }
                    }
                }
            } else if (cellContent == '!') { // Caravana bárbara
                cout << "Caravana bárbara afetada pela tempestade em (" << wrappedRow << ", " << wrappedCol << ")." << endl;
                for (auto caravan : caravans) {
                    if (caravan->getRow() == wrappedRow && caravan->getCol() == wrappedCol) {
                        caravan->loseCrew(10); // Reduz tripulantes
                        if (!caravan->isActive()) {
                            caravan->becomeObstacle(map); // Torna-se obstáculo se inativa
                        }
                    }
                }
            } else {
                cout << "Nenhum elemento afetado em (" << wrappedRow << ", " << wrappedCol << ")." << endl;
            }
        }
    }
}