#include "Simulator.h"
#include <iostream>

using namespace std;

void Simulator::saveBufferState(const std::string &nome) {
    cout << "Guardar estado do buffer com nome " << nome << " (não implementado)." << endl;
}

void Simulator::loadBufferState(const std::string &nome) {
    cout << "Carregar estado do buffer com nome " << nome << " (não implementado)." << endl;
}

void Simulator::listSavedStates() const {
    cout << "Listar estados guardados (não implementado)." << endl;
}

void Simulator::deleteSavedState(const std::string &nome) {
    cout << "Apagar estado guardado " << nome << " (não implementado)." << endl;
}