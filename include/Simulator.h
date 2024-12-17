#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <unordered_map>
#include "Map.h"
#include "Caravan.h"
#include <vector>
#include "Buffer.h"
#include "Wallet.h"

class Simulator {
private:
    Map map; // Objeto do tipo Map
    std::vector<Caravan*> caravans; // Lista de caravanas
    int bufferRows, bufferCols; // Tamanho do buffer
    int mapRows, mapCols;
    Buffer buffer;
    Wallet wallet;

public:
    Simulator();

    // Adiciona uma caravana no mapa
    void addCaravan(Caravan* caravan, int row, int col);

    // Função principal do simulador
    void run();

    // Atualiza o estado do mapa
    void displayMap();

    bool loadMap(const std::string& filename);

    bool moveCaravan(int caravanId, char direction);

    void showCaravanStatus() const;

    // Map para armazenar o estado original de células ocupadas pelas caravanas
    std::unordered_map<int, char> originalCellContent;

};

#endif
