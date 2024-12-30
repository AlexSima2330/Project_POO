#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <unordered_map>
#include "Map.h"
#include "Caravan.h"
#include <vector>

#include "BarbarianCaravan.h"
#include "Buffer.h"
#include "Item.h"
#include "Wallet.h"

class Buffer;

class Simulator {
private:
    Map map;
    std::vector<Caravan*> caravans;
    int bufferRows, bufferCols;
    int mapRows, mapCols;
    Buffer buffer;
    Wallet wallet;
    int elapsedInstants = 0;
    int totalCombatsWon = 0;
    std::vector<Caravan*> simulatorCaravans;
    std::vector<Item*> items;
    int itemDuration;
    int maxItems;
    int timeBetweenItems;
    int timeBetweenBarbarians;
    int barbarianDuration;
    bool shouldEndSimulation();
    std::unordered_map<std::string, Buffer> savedBuffers;

public:
    Simulator();

    void run();

    bool loadMap(const std::string& filename);
    void displayMap();

    void addCaravan(Caravan* caravan, int row, int col);
    void removeCaravan(Caravan* caravan);
    bool moveCaravan(int caravanId, char direction);
    void showCaravanStatus() const;

    void advanceSimulation(int n);
    void showCaravanStatus();
    void showPrices() const;
    void addCoins(int n);

    void listCityCaravans(char cityName) const;
    void createSandstorm(int l, int c, int r);

    void handleTradeCaravanInSandstorm(TradeCaravan* caravan);
    void handleMilitaryCaravanInSandstorm(MilitaryCaravan* caravan);


    void buyCaravan(char cityName, char tipo);
    void showCaravanDetails(int caravanId) const;
    void buyMerchandise(int caravanId, int amount);
    void sellMerchandise(int caravanId);
    void moveCaravanWithDirection(int caravanId, const std::string &direction);
    void setCaravanAuto(int caravanId);
    void handleTradeCaravanAuto(Caravan* caravan);
    void handleMilitaryCaravanAuto(Caravan* caravan);
    void handleSecretCaravanAuto(Caravan* caravan);
    void handleCaravanWithoutCrew(Caravan* caravan);
    void stopCaravanAuto(int caravanId);
    void createBarbarianCaravan(int l, int c);
    void buyCrewForCaravan(int caravanId, int crewCount);

    void saveBufferState(const std::string &nome);
    void loadBufferState(const std::string &nome);
    void listSavedStates() const;
    void deleteSavedState(const std::string &nome);

    void handleCombatResult(bool playerWon);
    void initializeCityCaravans();
    void checkAndEndSimulation();

    const std::vector<Caravan*>& getCaravans() const { return caravans; }

    void spawnItem();
    void updateItems();
    void checkCaravanForItem(Caravan* caravan);

    const Wallet& getWallet() const;
    Wallet& getWallet();
    void removeCaravanMine(Caravan* caravan);

    void handleBarbarianCaravanAuto(BarbarianCaravan* caravan);
    void spawnBarbarianCaravan();

    void resolveCombats();
    void incrementCombatWins() {
        totalCombatsWon++;
    }

    void endSimulation();
};

#endif
