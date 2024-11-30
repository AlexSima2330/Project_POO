#include "Map.h"
#include <fstream>
#include <stdexcept>

// Construtor: inicializa o mapa com '.' (espaço vazio)
Map::Map(int rows, int cols) : rows(rows), cols(cols), grid(rows, std::vector<char>(cols, '.')) {}

// Função para carregar o mapa de um ficheiro
bool Map::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o ficheiro: " << filename << std::endl;
        return false;
    }

    // Lê as dimensões
    file >> rows >> cols;
    grid.resize(rows, std::vector<char>(cols, '.')); // Ajusta o tamanho do mapa

    // Lê o conteúdo do mapa
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file >> grid[i][j];
        }
    }

    file.close();
    return true;
}

// Função para exibir o mapa
void Map::display() const {
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

// Getter para obter o conteúdo de uma célula
char Map::getCell(int row, int col) const {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        return grid[row][col];
    }
    throw std::out_of_range("Coordenadas fora dos limites do mapa.");
}

// Setter para modificar o conteúdo de uma célula
void Map::setCell(int row, int col, char value) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        grid[row][col] = value;
    } else {
        throw std::out_of_range("Coordenadas fora dos limites do mapa.");
    }
}
