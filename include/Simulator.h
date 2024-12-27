#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <unordered_map>
#include "Map.h"
#include "Caravan.h"
#include <vector>
#include "Buffer.h"
#include "Wallet.h"

class Simulator {
private:
    Map map; // Objeto do tipo Map
    std::vector<Caravan*> caravans; // Lista de caravanas
    int bufferRows, bufferCols; // Tamanho do buffer
    int mapRows, mapCols;
    Buffer buffer;
    Wallet wallet;
    int elapsedInstants = 0; // Contador de instantes
    int totalCombatsWon = 0; // Contador de combates vencidos
    std::vector<Caravan*> simulatorCaravans;
    bool shouldEndSimulation();

public:
    Simulator();

    void run();

    // Métodos fase 1 / mapa
    bool loadMap(const std::string& filename);
    void displayMap();

    // Métodos de caravanas
    void addCaravan(Caravan* caravan, int row, int col);
    void removeCaravan(Caravan* caravan);
    bool moveCaravan(int caravanId, char direction);
    void showCaravanStatus() const;

    // Métodos da fase 2 (organização conforme já discutido):

    // Geral (podes deixar no Simulator.cpp)
    void advanceSimulation(int n);
    void showCaravanStatus();
    void showPrices() const;
    void addCoins(int n);

    // Map/ambiente (Simulator_MapActions.cpp)
    void listCityCaravans(char cityName) const;
    void createSandstorm(int l, int c, int r);

    void handleTradeCaravanInSandstorm(TradeCaravan* caravan);
    void handleMilitaryCaravanInSandstorm(MilitaryCaravan* caravan);


    // Caravanas (Simulator_CaravanActions.cpp)
    void buyCaravan(char cityName, char tipo);
    void showCaravanDetails(int caravanId) const;
    void buyMerchandise(int caravanId, int amount);
    void sellMerchandise(int caravanId);
    void moveCaravanWithDirection(int caravanId, const std::string &direction);
    void setCaravanAuto(int caravanId);
    void handleTradeCaravanAuto(Caravan* caravan);
    void handleMilitaryCaravanAuto(Caravan* caravan);
    void handleCaravanWithoutCrew(Caravan* caravan);
    void stopCaravanAuto(int caravanId);
    void createBarbarianCaravan(int l, int c);
    void buyCrewForCaravan(int caravanId, int crewCount);


    // Storage (Simulator_Storage.cpp)
    void saveBufferState(const std::string &nome);
    void loadBufferState(const std::string &nome);
    void listSavedStates() const;
    void deleteSavedState(const std::string &nome);

    void handleCombatResult(bool playerWon);
    void initializeCityCaravans();
    void checkAndEndSimulation();

    const std::vector<Caravan*>& getCaravans() const { return caravans; }


    void endSimulation();
};

#endif
