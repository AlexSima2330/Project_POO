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
        int row, col;

    public:
        City(char name, int row, int col);
        int getRow() const { return row; }
        int getCol() const { return col; }


        //metodos basicos
        char getName() const {return name; }
        void addCaravan(Caravan* caravan);
        void removeCaravan(Caravan* caravan);
        const vector<Caravan*>& getCaravans() const;
        vector<Caravan*>& getCaravans();

        // metodos de interaçao
        void listCaravans() const;
        void sellGoods(Caravan* caravan, int quantity, int pricePerTon);
        void buyGoods(Caravan* caravan, int quantity, int pricePerTon);
        void recruitCrew(Caravan* caravan, int quantity);
        void initializeCaravans(int& globalCaravanID);

};

#endif //CITY_H
