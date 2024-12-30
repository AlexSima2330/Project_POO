#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include "City.h"

using namespace std;

class City;

class Map {
private:
    int rows;
    int cols;
    vector<char> grid;
    vector<City> cities;
    int index(int row, int col) const;

public:
    Map(int rows, int cols);
    std::vector<City>& getCities();
    const std::vector<City>& getCities() const;

    pair<int, int> wrapCoordinates(int row, int col) const;

    bool loadFromFile(const string& filename);

    void display() const;

    char getCell(int row, int col) const;

    void setCell(int row, int col, char value);

    void addCity(char name, int row, int col);
    City* getCityByName(char name) const;
    City* getCityAt(int row, int col);
    bool isCity(int row, int col) const;

    int getRows() const { return rows; }
    int getCols() const { return cols; }
};

#endif
