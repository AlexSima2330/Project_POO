#include "Caravan.h"
#include <iostream>

Caravan::Caravan(int id, const std::string &type, int maxCargo, int maxWater)
    : id(id), type(type), crew(10), cargo(0), water(maxWater), maxCargo(maxCargo), maxWater(maxWater) {}

void Caravan::move(const std::string &direction) {
    std::cout << "Caravan " << id << " moving " << direction << std::endl;
}

void Caravan::status() const {
    std::cout << "Caravan " << id << " (" << type << "): Crew=" << crew
              << ", Cargo=" << cargo << "/" << maxCargo
              << ", Water=" << water << "/" << maxWater << std::endl;
}

TradeCaravan::TradeCaravan(int id) : Caravan(id, "Trade", 40, 200) {}

void TradeCaravan::move(const std::string &direction) {
    std::cout << "Trade Caravan " << id << " moving " << direction << std::endl;
}

MilitaryCaravan::MilitaryCaravan(int id) : Caravan(id, "Military", 5, 400) {}

void MilitaryCaravan::move(const std::string &direction) {
    std::cout << "Military Caravan " << id << " moving " << direction << std::endl;
}
