#include <City.h>
#include <iostream>
#include <algorithm>

using namespace std;

// Construtor
City::City(char name) : name(name) {}

// Adicionar uma caravana à cidade
void City::addCaravan(Caravan* caravan) {
    caravans.push_back(caravan);
}

// Remover uma caravana da cidade
void City::removeCaravan(Caravan* caravan) {
    caravans.erase(std::remove(caravans.begin(), caravans.end(), caravan), caravans.end());
}


// Listar todas as caravanas na cidade
void City::listCaravans() const {
    std::cout << "Caravanas na cidade " << name << ":" << std::endl;
    for (const auto& caravan : caravans) {
        std::cout << " - Caravana ID: " << caravan->getId() << std::endl;
    }
}

void City::sellGoods(Caravan* caravan, int quantity, int pricePerTon) {
    if (caravan->getCargo() < quantity) {
        std::cout << "Erro: Caravana ID " << caravan->getId() << " não tem mercadoria suficiente para vender." << std::endl;
        return;
    }

    caravan->setCargo(caravan->getCargo() - quantity);
    std::cout << "Caravana ID " << caravan->getId() << " vendeu " << quantity
              << " toneladas de mercadoria por " << (quantity * pricePerTon) << " moedas." << std::endl;
}

void City::buyGoods(Caravan* caravan, int quantity, int pricePerTon) {
    if (caravan->getMaxCargo() - caravan->getCargo() < quantity) {
        std::cout << "Erro: Caravana ID " << caravan->getId() << " não tem espaço suficiente para mais mercadoria." << std::endl;
        return;
    }

    caravan->setCargo(caravan->getCargo() + quantity);
    std::cout << "Caravana ID " << caravan->getId() << " comprou " << quantity
              << " toneladas de mercadoria por " << (quantity * pricePerTon) << " moedas." << std::endl;
}


// Recrutar tripulantes para uma caravana
void City::recruitCrew(Caravan* caravan, int quantity) {
    // Lógica para recrutar tripulantes
}
