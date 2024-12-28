#ifndef WALLET_H
#define WALLET_H

class Wallet {
private:
    int coins;

public:
    Wallet();
    Wallet(int initialCoins);

    int getCoins() const;
    void addCoins(int amount);
    bool deductCoins(int amount);

    void addPercentageCoins(float percentage);
};

#endif
