#include "Simulator.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "Wallet.h"
#include "Map.h"
#include "Commands2.h" // se necessário

using namespace std;

Simulator::Simulator() : map(0, 0), bufferRows(0), bufferCols(0), buffer(0, 0), wallet(0) {
    cout << "Mapa criado com sucesso!" << endl;
}

void Simulator::run() {
    while (true) {
        bool mapLoaded = false;
        std::string command;

        // Fase 1
        while (!mapLoaded) {
            std::cout << "Digite 'config <ficheiro>' para carregar o mapa ou 'sair' para terminar: ";
            if (!std::getline(std::cin, command)) {
                return; // se EOF ou erro, sai do programa
            }

            if (command.find("config") == 0) {
                std::istringstream iss(command);
                std::string cmd, filename;
                iss >> cmd >> filename;

                if (!filename.empty()) {
                    if (loadMap(filename)) {
                        std::cout << "Mapa carregado com sucesso do ficheiro: " << filename << std::endl;

                        // Adicionar caravanas aqui, após o mapa ser carregado
                        Caravan* trade = new TradeCaravan(1);
                        trade->setPosition(2, 9);
                        addCaravan(trade, 2, 9);
                        trade->setWater(10);

                        Caravan* military = new MilitaryCaravan(2);
                        military->setPosition(7, 4);
                        addCaravan(military, 7, 4);
                        military->setWater(10);

                        mapLoaded = true;
                    } else {
                        std::cerr << "Falha ao carregar o mapa. Tente novamente." << std::endl;
                    }
                } else {
                    std::cerr << "Uso incorreto do comando. Exemplo: config <nomeFicheiro>" << std::endl;
                }
            } else if (command == "sair") {
                std::cout << "Simulacao terminada." << std::endl;
                return; // Sai do run, termina o programa
            } else {
                std::cerr << "Comando invalido nesta fase. Use 'config <ficheiro>' ou 'sair'." << std::endl;
            }
        }

        // Fase 2
        while (true) {
            displayMap();

            std::cout << "Digite um comando ou 'terminar' para voltar a fase 1.\n"
     << "Comandos disponiveis na fase 2:\n"
     << "  exec <ficheiro>   - Executa comandos a partir de um ficheiro\n"
     << "  prox <n>          - Avanca a simulacao n instantes\n"
     << "  comprac <C> <T>   - Compra uma caravana do tipo T na cidade C\n"
     << "  precos            - Lista os precos das mercadorias\n"
     << "  cidade <C>        - Lista o conteudo da cidade C\n"
     << "  caravana <N>      - Mostra a descricao da caravana N\n"
     << "  compra <N> <M>    - Compra M toneladas de mercadorias para a caravana N (na cidade)\n"
     << "  vende <N>         - Vende toda a mercadoria da caravana N (na cidade)\n"
     << "  move <N> <X>      - Move a caravana N na direcao X (D, E, C, B, CE, CD, BE, BD)\n"
     << "  auto <N>          - Coloca a caravana N em modo auto-gestao\n"
     << "  stop <N>          - Para o modo auto da caravana N\n"
     << "  barbaro <l> <c>   - Cria uma caravana barbara em (l, c)\n"
     << "  areia <l> <c> <r> - Cria uma tempestade de areia em (l,c) com raio r\n"
     << "  moedas <N>        - Acrescenta N moedas ao jogador (pode ser negativo)\n"
     << "  tripul <N> <T>    - Adiciona T tripulantes a caravana N (na cidade)\n"
     << "  saves <nome>      - Guarda o estado atual do buffer com o nome dado\n"
     << "  loads <nome>      - Carrega um estado anteriormente guardado\n"
     << "  lists             - Lista os nomes dos estados guardados\n"
     << "  dels <nome>       - Apaga um estado guardado pelo nome\n"
     << "  terminar          - Termina a simulacao e volta a fase 1\n"
     << "Digite o comando: ";

            if (!std::getline(std::cin, command)) {
                return; // se EOF, sai do programa
            }

            if (command.find("exec") == 0) {
                std::istringstream iss(command);
                std::string cmd, filename;
                iss >> cmd >> filename;

                if (!filename.empty()) {
                    std::ifstream file(filename);
                    if (!file.is_open()) {
                        std::cerr << "Não foi possivel abrir o ficheiro: " << filename << std::endl;
                    } else {
                        std::string line;
                        while (std::getline(file, line)) {
                            processPhase2Command(*this, line);
                        }
                    }
                } else {
                    std::cerr << "Uso incorreto: exec <nomeFicheiro>" << std::endl;
                }
            } else if (command == "terminar") {
                std::cout << "A simulacao terminou. Voltando a fase 1..." << std::endl;
                // Aqui não faz return, nem break do programa inteiro,
                // Apenas break do loop da fase 2, voltando ao loop externo do run()
                break;
            } else {
                processPhase2Command(*this, command);
            }
        }
    }
}


void Simulator::advanceSimulation(int n) {
    cout << "A avançar " << n << " instantes (não implementado)." << endl;
}

void Simulator::showPrices() const {
    cout << "Preços das mercadorias (não implementado)." << endl;
}

void Simulator::addCoins(int n) {
    cout << "Adicionar " << n << " moedas (não implementado)." << endl;
}