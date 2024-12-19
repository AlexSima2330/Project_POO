#include "Wallet.h"

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