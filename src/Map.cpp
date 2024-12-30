#include "Map.h"
#include "City.h"
#include <fstream>

using namespace std;

// Construtor: inicializa o mapa com '.' (espaço vazio)
Map::Map(int rows, int cols) : rows(rows), cols(cols), grid(rows * cols, '.') {}

// Método para calcular o índice no vetor unidimensional
int Map::index(int row, int col) const {
    auto [wrappedRow, wrappedCol] = wrapCoordinates(row, col);
    return wrappedRow * cols + wrappedCol;
}

// Método para coordenadas circulares
pair<int, int> Map::wrapCoordinates(int row, int col) const {
    if (col >= cols) {
        col = 0; // Volta para a primeira coluna
    } else if (col < 0) {
        col = cols - 1; // Vai para a última coluna
    }

    if (row >= rows) {
        row = 0; // Volta para a primeira linha
    } else if (row < 0) {
        row = rows - 1; // Vai para a última linha
    }

    return {row, col};
}

// Função para carregar o mapa de um ficheiro
bool Map::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o ficheiro: " << filename << endl;
        return false;
    }

    // Mantém as dimensões iniciais do mapa
    grid.assign(rows * cols, '.'); // Reseta o mapa com as dimensões especificadas

    for (int i = 0; i < rows && !file.eof(); ++i) {
        for (int j = 0; j < cols && !file.eof(); ++j) {
            char cell;
            file >> cell;
            grid[index(i, j)] = cell;
            if (std::islower(cell)) {  // Identifica cidades (letras minúsculas)
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


// Função para exibir o mapa
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
        std::cout << "- Cidade " << city.getName() << " na posição especificada." << std::endl;
    }
}

// Gerir cidades
void Map::addCity(char name, int row, int col) {
    cities.emplace_back(name, row, col);
    grid[index(row, col)] = name;
}

// Obter uma cidade pelo nome
City* Map::getCityByName(char name) const {
    for (const auto& city : cities) {
        if (city.getName() == name) {
            return const_cast<City*>(&city); // Remove constness do ponteiro
        }
    }
    return nullptr;
}

// Obter uma cidade pela posição
City* Map::getCityAt(int row, int col) {
    char cell = getCell(row, col);
    if (std::islower(cell)) {
        return getCityByName(cell);
    }
    return nullptr;
}

// Verificar se é uma cidade
bool Map::isCity(int row, int col) const {
    char cell = getCell(row, col);
    return std::islower(cell);
}

// Getter para obter o conteúdo de uma célula
char Map::getCell(int row, int col) const {
    return grid[index(row, col)];
}

// Setter para modificar o conteúdo de uma célula
void Map::setCell(int row, int col, char value) {
    if (std::islower(getCell(row, col)) && value != getCell(row, col)) {
        return;
    }
    grid[index(row, col)] = value;
}