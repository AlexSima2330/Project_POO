#include "Map.h"
#include "City.h"
#include <fstream>
#include <iostream>

using namespace std;

Map::Map(int rows, int cols) : rows(rows), cols(cols), grid(rows * cols, '.') {}

int Map::index(int row, int col) const {
    auto [wrappedRow, wrappedCol] = wrapCoordinates(row, col);
    return wrappedRow * cols + wrappedCol;
}

pair<int, int> Map::wrapCoordinates(int row, int col) const {
    if (col >= cols) {
        col = 0;
    } else if (col < 0) {
        col = cols - 1;
    }

    if (row >= rows) {
        row = 0;
    } else if (row < 0) {
        row = rows - 1;
    }

    return {row, col};
}

bool Map::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o ficheiro: " << filename << endl;
        return false;
    }

    grid.assign(rows * cols, '.');

    for (int i = 0; i < rows && !file.eof(); ++i) {
        for (int j = 0; j < cols && !file.eof(); ++j) {
            char cell;
            file >> cell;
            grid[index(i, j)] = cell;
            if (std::islower(cell)) {
                addCity(cell, i, j);
            }
        }
    }

    file.close();
    return true;
}

vector<City>& Map::getCities() {
    return cities;
}

const std::vector<City>& Map::getCities() const {
    return cities;
}

void Map::display() const {
    std::cout << "Mapa Atual:" << std::endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << grid[index(i, j)] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nCidades no mapa:" << std::endl;
    for (const auto& city : cities) {
        std::cout << "- Cidade " << city.getName() << " na posicao especificada." << std::endl;
    }
}

void Map::addCity(char name, int row, int col) {
    cities.emplace_back(name, row, col);
    grid[index(row, col)] = name;
}

City* Map::getCityByName(char name) const {
    for (const auto& city : cities) {
        if (city.getName() == name) {
            return const_cast<City*>(&city);
        }
    }
    return nullptr;
}

City* Map::getCityAt(int row, int col) {
    char cell = getCell(row, col);
    if (std::islower(cell)) {
        return getCityByName(cell);
    }
    return nullptr;
}

bool Map::isCity(int row, int col) const {
    char cell = getCell(row, col);
    return std::islower(cell);
}

char Map::getCell(int row, int col) const {
    return grid[index(row, col)];
}

void Map::setCell(int row, int col, char value) {
    if (std::islower(getCell(row, col)) && value != getCell(row, col)) {
        return;
    }
    grid[index(row, col)] = value;
}