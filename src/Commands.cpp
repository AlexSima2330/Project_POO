//
// Created by Alexa on 18/12/2024.
//
#include "Commands.h"
#include "Simulator.h"
#include <iostream>
#include <sstream>

void processPhase2Command(Simulator &sim, const std::string &command) {
    if (command.find("prox") == 0) {
        std::istringstream iss(command);
        std::string cmd; iss >> cmd;
        int n = 1;
        if (!(iss >> n)) n = 1;
        if (n <= 0) n = 1;
        sim.advanceSimulation(n);
    } else if (command.find("comprac") == 0) {
        std::istringstream iss(command);
        std::string cmd; iss >> cmd;
        char cityName, tipo;
        iss >> cityName >> tipo;
        sim.buyCaravan(cityName, tipo);
    } else if (command == "precos") {
        sim.showPrices();
    } else if (command.find("cidade") == 0) {
        std::istringstream iss(command);
        std::string cmd; iss >> cmd;
        char cityName;
        iss >> cityName;
        sim.listCityCaravans(cityName);
    } else if (command.find("caravana") == 0) {
        std::istringstream iss(command);
        std::string cmd;
        int caravanId;
        iss >> cmd >> caravanId;
        sim.showCaravanDetails(caravanId);
    } else if (command.find("compra") == 0 && command.substr(0,6) == "compra ") {
        std::istringstream iss(command);
        std::string cmd; iss >> cmd;
        int caravanId, m;
        iss >> caravanId >> m;
        sim.buyMerchandise(caravanId, m);
    } else if (command.find("vende") == 0) {
        std::istringstream iss(command);
        std::string cmd; iss >> cmd;
        int caravanId;
        iss >> caravanId;
        sim.sellMerchandise(caravanId);
    } else if (command.find("move") == 0) {
        std::istringstream iss(command);
        std::string cmd; iss >> cmd;
        int caravanId;
        std::string direction;
        iss >> caravanId >> direction;
        sim.moveCaravanWithDirection(caravanId, direction);
    } else if (command.find("auto") == 0 && command.substr(0,5) == "auto ") {
        std::istringstream iss(command);
        std::string cmd; iss >> cmd;
        int caravanId; iss >> caravanId;
        sim.setCaravanAuto(caravanId);
    } else if (command.find("stop") == 0) {
        std::istringstream iss(command);
        std::string cmd; iss >> cmd;
        int caravanId; iss >> caravanId;
        sim.stopCaravanAuto(caravanId);
    } else if (command.find("barbaro") == 0) {
        std::istringstream iss(command);
        std::string cmd; iss >> cmd;
        int l, c; iss >> l >> c;
        sim.createBarbarianCaravan(l, c);
    } else if (command.find("areia") == 0) {
        std::istringstream iss(command);
        std::string cmd; iss >> cmd;
        int l, c, r;
        iss >> l >> c >> r;
        sim.createSandstorm(l, c, r);
    } else if (command.find("moedas") == 0) {
        std::istringstream iss(command);
        std::string cmd; iss >> cmd;
        int n; iss >> n;
        sim.addCoins(n);
    } else if (command.find("tripul") == 0) {
        std::istringstream iss(command);
        std::string cmd; iss >> cmd;
        int caravanId, t;
        iss >> caravanId >> t;
        sim.addCrewToCaravan(caravanId, t);
    } else if (command.find("saves") == 0) {
        std::istringstream iss(command);
        std::string cmd, nome; iss >> cmd >> nome;
        sim.saveBufferState(nome);
    } else if (command.find("loads") == 0) {
        std::istringstream iss(command);
        std::string cmd, nome; iss >> cmd >> nome;
        sim.loadBufferState(nome);
    } else if (command == "lists") {
        sim.listSavedStates();
    } else if (command.find("dels") == 0) {
        std::istringstream iss(command);
        std::string cmd, nome; iss >> cmd >> nome;
        sim.deleteSavedState(nome);
    } else {
        std::cout << "Comando inválido." << std::endl;
    }
}
