#include "Wallet.h"

Wallet::Wallet() : coins(0) {}
Wallet::Wallet(int initialCoins) : coins(initialCoins) {}

int Wallet::getCoins() const {
    return coins;
}

void Wallet::addCoins(int amount) {
    coins += amount;
}

bool Wallet::deductCoins(int amount) {
    if (coins >= amount) {
        coins -= amount;
        return true;
    }
    return false;
}