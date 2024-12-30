#include <City.h>
#include <iostream>
#include <algorithm>

using namespace std;

City::City(char name, int row, int col) : name(name), row(row), col(col) {}

void City::addCaravan(Caravan* caravan) {
    caravans.push_back(caravan);
}

void City::removeCaravan(Caravan* caravan) {
    caravans.erase(std::remove(caravans.begin(), caravans.end(), caravan), caravans.end());
}

void City::listCaravans() const {
    std::cout << "Caravanas na cidade " << name << ":" << std::endl;

    if (caravans.empty()) {
        std::cout << " - Nenhuma caravana disponível para compra." << std::endl;
        return;
    }

    for (const auto& caravan : caravans) {
        std::cout << " - Caravana ID: " << caravan->getId()
                  << ", Tipo: " << caravan->getType()
                  << ", Tripulacao: " << caravan->getCrew()
                  << ", Carga: " << caravan->getCargo()
                  << ", Agua: " << caravan->getWater();

        if (caravan->isOwned()) {
            std::cout << " (Comprada e estacionada)";
        } else {
            std::cout << " (Disponivel para compra)";
        }

        std::cout << std::endl;
    }
}

void City::sellGoods(Caravan* caravan, int quantity, int pricePerTon) {
    if (caravan->getCargo() < quantity) {
        std::cout << "Erro: Caravana ID " << caravan->getId() << " nao tem mercadoria suficiente para vender." << std::endl;
        return;
    }

    caravan->setCargo(caravan->getCargo() - quantity);
    std::cout << "Caravana ID " << caravan->getId() << " vendeu " << quantity
              << " toneladas de mercadoria por " << (quantity * pricePerTon) << " moedas." << std::endl;
}

void City::buyGoods(Caravan* caravan, int quantity, int pricePerTon) {
    if (caravan->getMaxCargo() - caravan->getCargo() < quantity) {
        std::cout << "Erro: Caravana ID " << caravan->getId() << " nao tem espaço suficiente para mais mercadoria." << std::endl;
        return;
    }

    caravan->setCargo(caravan->getCargo() + quantity);
    std::cout << "Caravana ID " << caravan->getId() << " comprou " << quantity
              << " toneladas de mercadoria por " << (quantity * pricePerTon) << " moedas." << std::endl;
}

const std::vector<Caravan*>& City::getCaravans() const {
    return caravans;
}

std::vector<Caravan*>& City::getCaravans() {
    return caravans;
}


void City::initializeCaravans(int& globalCaravanID) {
    TradeCaravan* trade = new TradeCaravan(globalCaravanID++, 20);
    trade->setPosition(row, col);
    trade->setInCity(true);
    addCaravan(trade);

    MilitaryCaravan* military = new MilitaryCaravan(globalCaravanID++, 40);
    military->setPosition(row, col);
    military->setInCity(true);
    addCaravan(military);

    SecretCaravan* secret = new SecretCaravan(globalCaravanID++, 25);
    secret->setPosition(row, col);
    secret->setInCity(true);
    addCaravan(secret);
}