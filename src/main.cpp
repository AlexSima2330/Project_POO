#include "Simulator.h"
#include "Caravan.h"


int main() {
    Simulator simulator(10, 10, 10, 40);

    Caravan *trade = new TradeCaravan(1);
    Caravan *military = new MilitaryCaravan(2);

    simulator.addCaravan(trade, 2, 2);
    simulator.addCaravan(military, 3, 4);

    simulator.run();

    delete trade;
    delete military;

    return 0;
}
