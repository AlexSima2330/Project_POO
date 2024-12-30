#include "Simulator.h"
#include <iostream>

using namespace std;

// Guarda o estado atual do buffer com um nome
void Simulator::saveBufferState(const std::string& name) {
    savedBuffers[name] = buffer; // Copia o estado atual do buffer
    std::cout << "Estado do buffer guardado como '" << name << "'." << std::endl;
}

// Carrega um estado previamente guardado
void Simulator::loadBufferState(const std::string& name) {
    auto it = savedBuffers.find(name);
    if (it != savedBuffers.end()) {
        buffer = it->second; // Restaura o estado do buffer
        buffer.printToConsole();
        std::cout << "Estado do buffer '" << name << "' carregado." << std::endl;
    } else {
        std::cout << "Erro: Nenhum estado guardado com o nome '" << name << "'." << std::endl;
    }
}

// Lista todos os nomes dos estados guardados
void Simulator::listSavedStates() const {
    if (savedBuffers.empty()) {
        std::cout << "Nenhum estado do buffer guardado." << std::endl;
    } else {
        std::cout << "Estados guardados:" << std::endl;
        for (const auto& pair : savedBuffers) {
            std::cout << "- " << pair.first << std::endl;
        }
    }
}

// Apaga um estado guardado
void Simulator::deleteSavedState(const std::string& name) {
    auto it = savedBuffers.find(name);
    if (it != savedBuffers.end()) {
        savedBuffers.erase(it);
        std::cout << "Estado do buffer '" << name << "' apagado." << std::endl;
    } else {
        std::cout << "Erro: Nenhum estado guardado com o nome '" << name << "'." << std::endl;
    }
}