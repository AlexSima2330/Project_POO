#include "Simulator.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "Wallet.h"
#include "Map.h"
#include "Commands.h"

using namespace std;

Simulator::Simulator() : map(0, 0), bufferRows(0), bufferCols(0), buffer(0, 0), wallet(0) {
    cout << "Mapa criado com sucesso!" << endl;
}

void Simulator::run() {
    while (true) {
        bool mapLoaded = false;
        string command;

        // Fase 1
        while (!mapLoaded) {
            cout << "Digite 'config <ficheiro>' para carregar o mapa ou 'sair' para terminar: ";
            if (!getline(cin, command)) {
                return; // se EOF ou erro, sai do programa
            }

            if (command.find("config") == 0) {
                istringstream iss(command);
                string cmd, filename;
                iss >> cmd >> filename;

                if (!filename.empty()) {
                    if (loadMap(filename)) {
                        cout << "Mapa carregado com sucesso do ficheiro: " << filename << endl;

                        // Adicionar caravanas aqui, após o mapa ser carregado
                        Caravan* trade = new TradeCaravan(1, 20);
                        trade->setPosition(2, 9);
                        addCaravan(trade, 2, 9);
                        //trade->setWater(10);

                        Caravan* military = new MilitaryCaravan(2, 40);
                        military->setPosition(7, 4);
                        addCaravan(military, 7, 4);
                        //military->setWater(10);

                        trade->processMovement(map);     // Para a TradeCaravan
                        military->processMovement(map); // Para a MilitaryCaravan

                        mapLoaded = true;
                    } else {
                        cerr << "Falha ao carregar o mapa. Tente novamente." << endl;
                    }
                } else {
                    cerr << "Uso incorreto do comando. Exemplo: config <nomeFicheiro>" << endl;
                }
            } else if (command == "sair") {
                cout << "Simulacao terminada." << endl;
                return; // Sai do run, termina o programa
            } else {
               cerr << "Comando invalido nesta fase. Use 'config <ficheiro>' ou 'sair'." << endl;
            }
        }

        // Fase 2
        while (true) {
            displayMap();

            cout << "Digite um comando ou 'terminar' para voltar a fase 1.\n"
     << "Comandos disponiveis na fase 2:\n"
     //<< "  exec <ficheiro>   - Executa comandos a partir de um ficheiro\n"
     << "  prox <n>          - Avanca a simulacao n instantes\n"
     //<< "  comprac <C> <T>   - Compra uma caravana do tipo T na cidade C\n"
     //<< "  precos            - Lista os precos das mercadorias\n"
     //<< "  cidade <C>        - Lista o conteudo da cidade C\n"
     << "  caravana <N>      - Mostra a descricao da caravana N\n"
     //<< "  compra <N> <M>    - Compra M toneladas de mercadorias para a caravana N (na cidade)\n"
     //<< "  vende <N>         - Vende toda a mercadoria da caravana N (na cidade)\n"
     << "  move <N> <X>      - Move a caravana N na direcao X (D, E, C, B, CE, CD, BE, BD)\n"
     //<< "  auto <N>          - Coloca a caravana N em modo auto-gestao\n"
     //<< "  stop <N>          - Para o modo auto da caravana N\n"
     << "  barbaro <l> <c>   - Cria uma caravana barbara em (l, c)\n"
     //<< "  areia <l> <c> <r> - Cria uma tempestade de areia em (l,c) com raio r\n"
     << "  moedas <N>        - Acrescenta N moedas ao jogador (pode ser negativo)\n"
     //<< "  tripul <N> <T>    - Adiciona T tripulantes a caravana N (na cidade)\n"
     //<< "  saves <nome>      - Guarda o estado atual do buffer com o nome dado\n"
    // << "  loads <nome>      - Carrega um estado anteriormente guardado\n"
    // << "  lists             - Lista os nomes dos estados guardados\n"
   //  << "  dels <nome>       - Apaga um estado guardado pelo nome\n"
     << "Digite o comando: ";

            if (!getline(cin, command)) {
                return; // se EOF, sai do programa
            }

            if (command.find("exec") == 0) {
                istringstream iss(command);
                string cmd, filename;
                iss >> cmd >> filename;

                if (!filename.empty()) {
                    ifstream file(filename);
                    if (!file.is_open()) {
                        cerr << "Não foi possivel abrir o ficheiro: " << filename << endl;
                    } else {
                        string line;
                        while (getline(file, line)) {
                            processPhase2Command(*this, line);
                        }
                    }
                } else {
                    cerr << "Uso incorreto: exec <nomeFicheiro>" << endl;
                }
            } else if (command == "terminar") {
                cout << "A simulacao terminou. Voltando a fase 1..." << endl;
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
    for (int i = 0; i < n; ++i) {
        // Reseta movimentos de todas as caravanas
        for (auto caravan : caravans) {
            caravan->resetMoves();
        }

        // Processa comportamentos automáticos (se existirem)
        std::cout << "Avançou " << (i + 1) << " instante(s)." << std::endl;
    }
}

void Simulator::showPrices() const {
    cout << "Preços das mercadorias (não implementado)." << endl;
}

void Simulator::addCoins(int n) {
    wallet.addCoins(n); // Usa a funcionalidade da Wallet
    cout << "Moedas atualizadas. Novo saldo: " << wallet.getCoins() << endl;
}
