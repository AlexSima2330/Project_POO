#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <iostream>
#include "City.h"

using namespace std;

class City;

class Map {
private:
    int rows;
    int cols;
    vector<std::vector<char>> grid;
    vector<City> cities;

public:
    Map(int rows, int cols);

    // Método para ajustar coordenadas para o comportamento circular
    pair<int, int> wrapCoordinates(int row, int col) const;

    // Função para carregar o mapa de um ficheiro
    bool loadFromFile(const string& filename);

    // Função para exibir o mapa
    void display() const;

    // Getter para verificar o conteúdo de uma célula
    char getCell(int row, int col) const;

    // Setter para modificar o conteúdo de uma célula
    void setCell(int row, int col, char value);

    // Métodos para gerir cidades
    void addCity(char name, int row, int col);
    City* getCityByName(char name);
    City* getCityAt(int row, int col);
    bool isCity(int row, int col) const;

    // Getters para dimensões
    int getRows() const { return rows; }
    int getCols() const { return cols; }
};

#endif
