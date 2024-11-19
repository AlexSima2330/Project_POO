//
// Created by Alexa on 19/11/2024.
//

#include "Buffer.h"
#include <iostream>

// Construtor
Buffer::Buffer() {
    size = 0; // Inicialização básica
}

// Destrutor
Buffer::~Buffer() {
    // Código de limpeza, se necessário
}

// Implementação dos métodos públicos
void Buffer::initialize() {
    std::cout << "Buffer initialized!" << std::endl;
}
