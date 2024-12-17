#include "Simulator.h"
#include "Caravan.h"

int main() {
    Simulator simulator;

    // Carregar o mapa
    if (!simulator.loadMap("C:/Users/Alexa/OneDrive/Documentos/Project_POO/config/map_config.txt")) {
        return 1;
    }

    // Adicionar caravanas
    Caravan* trade = new TradeCaravan(1);
    trade->setPosition(2, 9);
    simulator.addCaravan(trade, 2, 9);
    trade->setWater(10);

    Caravan* military = new MilitaryCaravan(2);
    military->setPosition(7, 4);
    simulator.addCaravan(military, 7, 4);
    military->setWater(10);

    simulator.run();

    delete trade;
    delete military;

    return 0;
}
