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

public:
    Simulator();

    void run();

    // Métodos fase 1 / mapa
    bool loadMap(const std::string& filename);
    void displayMap();

    // Métodos de caravanas
    void addCaravan(Caravan* caravan, int row, int col);
    bool moveCaravan(int caravanId, char direction);
    void showCaravanStatus() const;

    // Métodos da fase 2 (organização conforme já discutido):

    // Geral (podes deixar no Simulator.cpp)
    void advanceSimulation(int n);
    void showPrices() const;
    void addCoins(int n);

    // Map/ambiente (Simulator_MapActions.cpp)
    void listCityCaravans(char cityName) const;
    void createSandstorm(int l, int c, int r);

    // Caravanas (Simulator_CaravanActions.cpp)
    void buyCaravan(char cityName, char tipo);
    void showCaravanDetails(int caravanId) const;
    void buyMerchandise(int caravanId, int amount);
    void sellMerchandise(int caravanId);
    void moveCaravanWithDirection(int caravanId, const std::string &direction);
    void setCaravanAuto(int caravanId);
    void stopCaravanAuto(int caravanId);
    void createBarbarianCaravan(int l, int c);
    void addCrewToCaravan(int caravanId, int t);

    // Storage (Simulator_Storage.cpp)
    void saveBufferState(const std::string &nome);
    void loadBufferState(const std::string &nome);
    void listSavedStates() const;
    void deleteSavedState(const std::string &nome);
};

#endif
