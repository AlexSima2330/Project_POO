#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

class Map {
private:
    int rows, cols;
    std::vector<std::vector<char>> grid;

public:
    Map(int rows, int cols);
    void display() const;
    void placeCaravan(int row, int col, char symbol);
};

#endif
