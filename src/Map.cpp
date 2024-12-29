#include "Map.h"
#include "City.h"
#include <fstream>
#include <stdexcept>

using namespace std;

// Construtor: inicializa o mapa com '.' (espaço vazio)
Map::Map(int rows, int cols) : rows(rows), cols(cols), grid(rows, vector<char>(cols, '.')) {}

// Método para coordenadas espiral
pair<int, int> Map::wrapCoordinates(int row, int col) const {
    // Ajuste de colunas (mantendo a linha intacta)
    if (col >= cols) {
        col = 0; // Volta para a primeira coluna
    } else if (col < 0) {
        col = cols - 1; // Vai para a última coluna
    }

    // Ajuste de linhas (mantendo a coluna intacta)
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
    grid.assign(rows, vector<char>(cols, '.')); // Reseta o mapa com as dimensões especificadas

    // Lê o conteúdo do mapa do arquivo, limitando às dimensões definidas
    for (int i = 0; i < rows && !file.eof(); ++i) {
        for (int j = 0; j < cols && !file.eof(); ++j) {
            char cell;
            file >> cell;
            grid[i][j] = cell;
            if (std::islower(cell)) {  // Identifica cidades (letras minúsculas)
                addCity(cell, i, j);
            }
        }
    }

    file.close();
    return true;
}

// Função para exibir o mapa
void Map::display() const {
    std::cout << "Mapa Atual:" << std::endl;
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nCidades no mapa:" << std::endl;
    for (const auto& city : cities) {
        std::cout << "- Cidade " << city.getName() << " na posicao especificada." << std::endl;
    }
}

//Gerir cidades
void Map::addCity(char name, int row, int col) {
    cities.emplace_back(name, row, col);
    grid[row][col] = name; // Marca a posição no mapa com a letra da cidade
}

City* Map::getCityByName(char name) const {
    for (const auto& city : cities) {
        if (city.getName() == name) {
            return const_cast<City*>(&city); // Remove constness do ponteiro
        }
    }
    return nullptr;
}

vector<City>& Map::getCities() {
    return cities;
}

const std::vector<City>& Map::getCities() const {
    return cities;
}


City* Map::getCityAt(int row, int col) {
    char cell = getCell(row, col);
    if (std::islower(cell)) {  // Letras minúsculas representam cidades
        return getCityByName(cell);
    }
    return nullptr;  // Não é uma cidade
}

bool Map::isCity(int row, int col) const {
    char cell = getCell(row, col);
    return std::islower(cell);  // Letras minúsculas representam cidades
}

// Getter para obter o conteúdo de uma célula (usando coordenadas circulares)
char Map::getCell(int row, int col) const {
    auto [wrappedRow, wrappedCol] = wrapCoordinates(row, col);
    return grid[wrappedRow][wrappedCol];
}

// Setter para modificar o conteúdo de uma célula (usando coordenadas circulares)
void Map::setCell(int row, int col, char value) {
    auto [wrappedRow, wrappedCol] = wrapCoordinates(row, col);

    // Não substitui células de cidades
    if (std::islower(grid[wrappedRow][wrappedCol]) && value != grid[wrappedRow][wrappedCol]) {
        return;
    }

    grid[wrappedRow][wrappedCol] = value;
}