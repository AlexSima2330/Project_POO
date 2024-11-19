//
// Created by Alexa on 19/11/2024.
//

#include "Simulator.h"
#include <iostream>

Simulator::Simulator() {
    capacity = 0; // Inicialização básica
}

Simulator::~Simulator() {
    // Código de limpeza, se necessário
}

void Simulator::move() {
    std::cout << "Caravan is moving!" << std::endl;
}