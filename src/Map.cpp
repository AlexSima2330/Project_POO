#include "Map.h"
#include <iostream>

Map::Map(int rows, int cols) : rows(rows), cols(cols) {
    grid.resize(rows, std::vector<char>(cols, '.'));
}

void Map::display() const {
    for (const auto &row : grid) {
        for (char cell : row) {
            std::cout << cell << ' ';
        }
        std::cout << std::endl;
    }
}

void Map::placeCaravan(int row, int col, char symbol) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        grid[row][col] = symbol;
    }
}
