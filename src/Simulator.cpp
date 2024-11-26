#include "Simulator.h"
#include <iostream>

Simulator::Simulator(int mapRows, int mapCols, int bufferRows, int bufferCols)
    : map(mapRows, mapCols), buffer(bufferRows, bufferCols) {}

void Simulator::addCaravan(Caravan *caravan, int row, int col) {
    caravans.push_back(caravan);
    map.placeCaravan(row, col, 'C');
}

void Simulator::run() {
    std::cout << "Starting simulation..." << std::endl;
    map.display();
}
