#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Map.h"
#include "Caravan.h"
#include <vector>

class Simulator {
private:
    Map map; // Objeto do tipo Map
    std::vector<Caravan*> caravans; // Lista de caravanas
    int bufferRows, bufferCols; // Tamanho do buffer

public:
    Simulator(int mapRows, int mapCols, int bufferRows, int bufferCols);

    // Adiciona uma caravana no mapa
    void addCaravan(Caravan* caravan, int row, int col);

    // Função principal do simulador
    void run();

    // Atualiza o estado do mapa
    void displayMap();

    bool loadMap(const std::string& filename);

    bool moveCaravan(int caravanId, char direction);

    void showCaravanStatus() const;

};

#endif
