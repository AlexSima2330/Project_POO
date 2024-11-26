#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Map.h"
#include "Caravan.h"
#include "Buffer.h"

class Simulator {
private:
    Map map;
    Buffer buffer;
    std::vector<Caravan*> caravans;

public:
    Simulator(int mapRows, int mapCols, int bufferRows, int bufferCols);
    void addCaravan(Caravan *caravan, int row, int col);
    void run();
};

#endif
