#include "Simulator.h"
#include "Caravan.h"

int main() {
    Simulator simulator(10, 10, 10, 40);

    // Carregar o mapa
    if (!simulator.loadMap("C:/Users/Alexa/OneDrive/Documentos/Project_POO/config/map_config.txt")) {
        std::cerr << "Erro ao carregar o mapa. A sair..." << std::endl;
        return 1;
    }

    // Adicionar caravanas
    Caravan* trade = new TradeCaravan(1);
    trade->setPosition(2, 5);
    simulator.addCaravan(trade, 2, 5);
    trade->setWater(100);    // Define 50 unidades de água para a caravana de comércio// Define 30 unidades de água para a caravana militar


    Caravan* military = new MilitaryCaravan(2);
    military->setPosition(7, 4);
    simulator.addCaravan(military, 7, 4);
    military->setWater(100);

    // Mostrar status inicial
    //simulator.showCaravanStatus();

    // Mover caravanas ao iniciar
    //simulator.moveCaravan(1, 'B'); // Move a caravana de comércio para baixo
    //simulator.moveCaravan(2, 'D'); // Move a caravana militar para a direita

    // Mostrar status após movimento
    //simulator.showCaravanStatus();

    simulator.run();

    delete trade;
    delete military;

    return 0;
}
