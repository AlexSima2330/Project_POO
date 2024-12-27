#include "Item.h"
#include <iostream>

Item::Item(ItemType type, int row, int col, int duration)
    : type(type), row(row), col(col), remainingTurns(duration) {}

int Item::getRow() const { return row; }
int Item::getCol() const { return col; }
ItemType Item::getType() const { return type; }

void Item::decreaseLifetime() {
    if (remainingTurns > 0) remainingTurns--;
}

bool Item::isExpired() const {
    return remainingTurns <= 0;
}

void Item::applyEffect(Caravan* caravan) {
    switch (type) {
        case ItemType::PandoraBox:
            caravan->loseCrew(caravan->getCrew() * 0.2); // Perde 20% dos tripulantes
        std::cout << "[Item] Caixa de Pandora: Caravana perdeu 20% da tripulação.\n";
        break;
        case ItemType::TreasureChest:
            std::cout << "[Item] Arca do Tesouro: Ganhou 10% mais moedas!\n";
        break;
        case ItemType::Cage:
            caravan->setCrew(caravan->getCrew() + 5); // Adiciona 5 tripulantes
        std::cout << "[Item] Jaula: +5 tripulantes (até o máximo).\n";
        break;
        case ItemType::Mine:
            std::cout << "[Item] Mina: Caravana destruída!\n";
        caravan->loseCrew(caravan->getCrew()); // Perde todos os tripulantes
        break;
        case ItemType::Surprise:
            std::cout << "[Item] Surpresa: Algo inesperado aconteceu!\n";
        break;
    }
}
