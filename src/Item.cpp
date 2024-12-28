#include "Item.h"
#include "Caravan.h"
#include "Simulator.h"
#include <iostream>

// Construtor
Item::Item(ItemType type, int row, int col, int duration)
    : type(type), row(row), col(col), remainingTurns(duration) {}

// Getter para tipo
ItemType Item::getType() const {
    return type;
}

// Getter para linha
int Item::getRow() const {
    return row;
}

// Getter para coluna
int Item::getCol() const {
    return col;
}

// Reduz a duração do item
void Item::decreaseLifetime() {
    if (remainingTurns > 0) {
        remainingTurns--;
    }
}

// Verifica se o item expirou
bool Item::isExpired() const {
    return remainingTurns <= 0;
}

// Aplica o efeito do item na caravana
void Item::applyEffect(Caravan* caravan, Simulator* simulator) {
    switch (type) {
        case ItemType::PandoraBox: {
            // Reduz 20% da tripulação
            int lostCrew = static_cast<int>(caravan->getCrew() * 0.2);
            caravan->loseCrew(lostCrew);
            std::cout << "[Item - Caixa de Pandora] A caravana perdeu " << lostCrew << " membros da tripulação devido a doenças.\n";
            break;
        }

        case ItemType::TreasureChest: {
            // Aumenta moedas em 10%
            int extraCoins = static_cast<int>(simulator->getWallet().getCoins() * 0.1);
            simulator->getWallet().addCoins(extraCoins);
            std::cout << "[Item - Arca do Tesouro] O jogador ganhou " << extraCoins << " moedas!\n";
            break;
        }

        case ItemType::Cage: {
            // Adiciona prisioneiros à tripulação (máximo não excedido)
            int addedCrew = 5; // Valor fixo
            int maxCrew = 100; // Substituir pelo limite real
            if (caravan->getCrew() + addedCrew > maxCrew) {
                addedCrew = maxCrew - caravan->getCrew();
            }
            caravan->setCrew(caravan->getCrew() + addedCrew);
            std::cout << "[Item - Jaula] Foram adicionados " << addedCrew << " membros à tripulação.\n";
            break;
        }

        case ItemType::Mine: {
            // Remove a caravana do simulador
            simulator->removeCaravan(caravan);
            std::cout << "[Item - Mina] A caravana foi destruída por uma mina e removida do simulador!\n";
            break;
        }

        case ItemType::Surprise: {
            std::cout << "[Item - Surpresa] A caravana foi resetada para os valores iniciais.\n";

            // Restaurar atributos padrão
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