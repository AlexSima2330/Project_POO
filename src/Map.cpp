#include "Map.h"
#include <fstream>
#include <stdexcept>

// Construtor: inicializa o mapa com '.' (espaço vazio)
Map::Map(int rows, int cols) : rows(rows), cols(cols), grid(rows, std::vector<char>(cols, '.')) {}

// Método para coordenadas espiral
std::pair<int, int> Map::wrapCoordinates(int row, int col) const {
    if (col >= cols) {
        // Saiu pela direita, move para a próxima linha, coluna inicial
        row = (row + 1) % rows;
        col = 0;
    } else if (col < 0) {
        // Saiu pela esquerda, move para a linha anterior, última coluna
        row = (row - 1 + rows) % rows;
        col = cols - 1;
    }

    if (row >= rows) {
        // Saiu pela última linha (indo para baixo), volta à primeira
        row = 0;
    } else if (row < 0) {
        // Saiu pela primeira linha (indo para cima), volta à última
        row = rows - 1;
    }

    return {row, col};
}


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

// Getter para obter o conteúdo de uma célula (usando coordenadas circulares)
char Map::getCell(int row, int col) const {
    auto [wrappedRow, wrappedCol] = wrapCoordinates(row, col);
    return grid[wrappedRow][wrappedCol];
}

// Setter para modificar o conteúdo de uma célula (usando coordenadas circulares)
void Map::setCell(int row, int col, char value) {
    auto [wrappedRow, wrappedCol] = wrapCoordinates(row, col);
    grid[wrappedRow][wrappedCol] = value;
}
