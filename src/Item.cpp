#include "Item.h"
#include "Caravan.h"
#include "Simulator.h"
#include <iostream>

Item::Item(ItemType type, int row, int col, int duration)
    : type(type), row(row), col(col), remainingTurns(duration) {}

ItemType Item::getType() const {
    return type;
}

int Item::getRow() const {
    return row;
}

int Item::getCol() const {
    return col;
}

void Item::decreaseLifetime() {
    if (remainingTurns > 0) {
        remainingTurns--;
    }
}

bool Item::isExpired() const {
    return remainingTurns <= 0;
}

void Item::applyEffect(Caravan* caravan, Simulator* simulator) {
    switch (type) {
        case ItemType::PandoraBox: {
            int lostCrew = static_cast<int>(caravan->getCrew() * 0.2);
            caravan->loseCrew(lostCrew);
            std::cout << "[Item - Caixa de Pandora] A caravana perdeu " << lostCrew << " membros da tripulacao devido a doencas.\n";
            break;
        }

        case ItemType::TreasureChest: {
            int extraCoins = static_cast<int>(simulator->getWallet().getCoins() * 0.1);
            simulator->getWallet().addCoins(extraCoins);
            std::cout << "[Item - Arca do Tesouro] O jogador ganhou " << extraCoins << " moedas!\n";
            break;
        }

        case ItemType::Cage: {
            int addedCrew = 5;
            int maxCrew = 100;
            if (caravan->getCrew() + addedCrew > maxCrew) {
                addedCrew = maxCrew - caravan->getCrew();
            }
            caravan->setCrew(caravan->getCrew() + addedCrew);
            std::cout << "[Item - Jaula] Foram adicionados " << addedCrew << " membros à tripulação.\n";
            break;
        }

        case ItemType::Mine: {
            simulator->removeCaravan(caravan);
            std::cout << "[Item - Mina] A caravana foi destruida por uma mina e removida do simulador!\n";
            break;
        }

        case ItemType::Surprise: {
            std::cout << "[Item - Surpresa] A caravana foi resetada para os valores iniciais.\n";

            // atributos padrão
            if (caravan->getType() == "Trade") {
                *caravan = TradeCaravan(caravan->getId(), caravan->getCrew());
            }
            else if (caravan->getType() == "Military") {
                *caravan = MilitaryCaravan(caravan->getId(), caravan->getCrew());
            }
            else if (caravan->getType() == "Secret") {
                *caravan = SecretCaravan(caravan->getId(), caravan->getCrew());
            }
            break;
        }

        default: {
            std::cout << "[Item] Tipo desconhecido, nenhum efeito aplicado.\n";
            break;
        }
    }
}