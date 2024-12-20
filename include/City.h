#ifndef CITY_H
#define CITY_H

#include <string>
#include <vector>
#include <iostream>
#include "Caravan.h"

using namespace std;

class Caravan;

class City {
    private:
        char name;
        vector<Caravan*> caravans;

    public:
    City(char name);

    //metodos basicos
    char getName() const {return name; }
    void addCaravan(Caravan* caravan);
    void removeCaravan(Caravan* caravan);
    vector<Caravan*> getCaravans() const {return caravans; }

    // metodos de interaçao
    void listCaravans() const;
    void sellGoods(Caravan* caravan, int quantity, int pricePerTon);
    void buyGoods(Caravan* caravan, int quantity, int pricePerTon);
    void recruitCrew(Caravan* caravan, int quantity);
};

#endif //CITY_H
