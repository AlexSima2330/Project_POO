#include "Wallet.h"

#include <iostream>

Wallet::Wallet() : coins(0) {}
Wallet::Wallet(int initialCoins) : coins(initialCoins) {}

int Wallet::getCoins() const {
    return coins;
}

void Wallet::addCoins(int amount) {
    coins += amount;
    if (coins < 0) coins = 0;
}

bool Wallet::deductCoins(int amount) {
    if (coins >= amount) {
        coins -= amount;
        return true;
    }
    return false;
}

// Adiciona uma percentagem das moedas atuais
void Wallet::addPercentageCoins(float percentage) {
    int bonus = static_cast<int>(coins * (percentage / 100.0f));
    addCoins(bonus);
    std::cout << "[Wallet] Adicionadas " << bonus << " moedas (bónus de " << percentage << "%).\n";
}