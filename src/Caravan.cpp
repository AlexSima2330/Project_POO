//
// Created by Alexa on 19/11/2024.
//

#include "Caravan.h"
#include <iostream>

Caravan::Caravan() {
    capacity = 0; // Inicialização básica
}

Caravan::~Caravan() {
    // Código de limpeza, se necessário
}

void Caravan::move() {
    std::cout << "Caravan is moving!" << std::endl;
}
