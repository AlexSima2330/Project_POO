//
// Created by Alexa on 19/11/2024.
//

#include "Map.h"
#include <iostream>

Map::Map() {
    capacity = 0; // Inicialização básica
}

Map::~Map() {
    // Código de limpeza, se necessário
}

void Map::move() {
    std::cout << "Caravan is moving!" << std::endl;
}