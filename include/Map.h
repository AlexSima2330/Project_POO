#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <iostream>

class Map {
private:
    int rows;
    int cols;
    std::vector<std::vector<char>> grid;

public:
    Map(int rows, int cols);

    // Função para carregar o mapa de um ficheiro
    bool loadFromFile(const std::string& filename);

    // Função para exibir o mapa
    void display() const;

    // Getter para verificar o conteúdo de uma célula
    char getCell(int row, int col) const;

    // Setter para modificar o conteúdo de uma célula
    void setCell(int row, int col, char value);

    // Getters para dimensões
    int getRows() const { return rows; }
    int getCols() const { return cols; }
};

#endif
