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

// Vender mercadorias a uma caravana
void City::sellGoods(Caravan* caravan, int quantity) {
    // Lógica para vender mercadorias, verifica capacidade da caravana
    // (Implementar com base na classe Caravan)
}

// Comprar mercadorias de uma caravana
void City::buyGoods(Caravan* caravan, int quantity) {
    // Lógica para comprar mercadorias
}

// Recrutar tripulantes para uma caravana
void City::recruitCrew(Caravan* caravan, int quantity) {
    // Lógica para recrutar tripulantes
}
