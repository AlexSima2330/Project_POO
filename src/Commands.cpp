#include "Commands.h"
#include "Simulator.h"
#include <iostream>
#include <sstream>

void processPhase2Command(Simulator &sim, const std::string &command) {
    if (command.find("prox") == 0) {
        std::istringstream iss(command);
        std::string cmd;
        int n = 1;
        iss >> cmd >> n;

        if (n <= 0) {
            std::cout << "Erro: O numero de instantes deve ser maior que 0." << std::endl;
        } else {
            sim.advanceSimulation(n);
        }
    } else if (command.find("comprac") == 0) {
        std::istringstream iss(command);
        std::string cmd;
        char cityName, type;
        iss >> cmd >> cityName >> type;

        if (!iss.fail()) {
            sim.buyCaravan(cityName, type);
        } else {
            std::cout << "Erro: Uso invalido do comando 'comprac'. Uso correto: comprac <C> <T>" << std::endl;
        }
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
    } else if (command.rfind("compra ", 0) == 0) {
        std::istringstream iss(command);
        std::string cmd;
        int caravanId, quantity;

        iss >> cmd >> caravanId >> quantity;
        if (!iss.fail() && !cmd.empty() && quantity > 0) {
            sim.buyMerchandise(caravanId, quantity);
        } else {
            std::cout << "Erro: Formato invalido para o comando 'compra'. Use: compra <caravanId> <quantidade>" << std::endl;
        }
    } else if (command.rfind("vende ", 0) == 0) {
        std::istringstream iss(command);
        std::string cmd;
        int caravanId;

        iss >> cmd >> caravanId;
        if (!iss.fail() && !cmd.empty()) {
            sim.sellMerchandise(caravanId);
        } else {
            std::cout << "Erro: Formato inválido para o comando 'vende'. Use: vende <caravanId>" << std::endl;
        }
    }
    else if (command.find("move") == 0) {
        std::istringstream iss(command);
        std::string cmd; iss >> cmd;
        int caravanId;
        std::string direction;
        iss >> caravanId >> direction;
        sim.moveCaravanWithDirection(caravanId, direction);
    } else if (command.find("auto") == 0 && command.substr(0,5) == "auto ") {
        std::istringstream iss(command);
        std::string cmd;
        int caravanId;
        iss >> cmd >> caravanId;
        sim.setCaravanAuto(caravanId);
    } else if (command.find("stop") == 0) {
        std::istringstream iss(command);
        std::string cmd;
        int caravanId;
        iss >> cmd >> caravanId;
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
        if (!iss.fail()) {
            sim.createSandstorm(l, c, r);
        } else {
            std::cout << "Erro: Comando 'areia' invalido. Uso: areia <linha> <coluna> <raio>" << std::endl;
        }
    } else if (command.find("moedas") == 0) {
        std::istringstream iss(command);
        std::string cmd; iss >> cmd;
        int n; iss >> n;

        sim.addCoins(n);
        sim.checkAndEndSimulation();
    } else if (command.find("tripul") == 0) {
        std::istringstream iss(command);
        std::string cmd;
        int caravanId, crewCount;
        iss >> cmd >> caravanId >> crewCount;

        if (!iss.fail() && crewCount > 0) {
            sim.buyCrewForCaravan(caravanId, crewCount);
        } else {
            std::cout << "Erro: Formato invalido para o comando 'tripul'. Uso: tripul <caravanId> <quantidade>" << std::endl;
        }
    }
    else if (command.find("saves") == 0) {
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
    }else if (command.find("saves") == 0) {
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
    } else if (command == "terminar") {
        sim.endSimulation();
        std::cout << "A simulacao terminou. Voltando a fase 1..." << std::endl;
        // Fim da simulação
    } else if (command.find("invisible") == 0) {
        std::istringstream iss(command);
        std::string cmd;
        int caravanId;
        iss >> cmd >> caravanId;

        for (auto& caravan : sim.getCaravans()) {
            if (caravan->getId() == caravanId && caravan->getType() == "Secret") {
                caravan->activateInvisibility();
                return;
            }
        }
        std::cout << "Erro: Caravana secreta com ID " << caravanId << " nao encontrada ou nao é do tipo Secret." << std::endl;
    } else {
        std::cout << "Comando invalido." << std::endl;
    }
}