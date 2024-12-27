#include "Simulator.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "Wallet.h"
#include "Map.h"
#include "Commands.h"
#include "Item.h"

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
                        initializeCityCaravans(); // Inicializa caravanas nas cidades
                        mapLoaded = true;
                        // Adicionar caravanas aqui, após o mapa ser carregado
                        //Caravan* trade = new TradeCaravan(1, 5);
                        //trade->setPosition(2, 7);
                        //addCaravan(trade, 2, 7);
                        //trade->setWater(10);

                        //Caravan* military = new MilitaryCaravan(2, 40);
                       // military->setPosition(7, 4);
                      //  addCaravan(military, 7, 4);
                        //military->setWater(10);

                        //trade->processMovement(map);     // Para a TradeCaravan
                        //military->processMovement(map); // Para a MilitaryCaravan

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

            if (shouldEndSimulation()) {
                endSimulation();
                return; // Sai completamente da simulação
            }

            cout << "Digite um comando ou 'terminar' para voltar a fase 1.\n"
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
     //<< "  saves <nome>      - Guarda o estado atual do buffer com o nome dado\n"
    // << "  loads <nome>      - Carrega um estado anteriormente guardado\n"
    // << "  lists             - Lista os nomes dos estados guardados\n"
   //  << "  dels <nome>       - Apaga um estado guardado pelo nome\n"
     << "Digite o comando: ";

            if (!getline(cin, command)) {
                return; // se EOF, sai do programa
            }

            if (command.find("exec") == 0) {
                std::istringstream iss(command);
                std::string cmd, filename;
                iss >> cmd >> filename;

                if (filename.empty()) {
                    filename = "run_commands.txt"; // Nome padrão
                }

                // Caminho do ficheiro na pasta "config"
                std::string filepath = "./config/" + filename;

                ifstream file(filepath);
                if (!file.is_open()) {
                    cerr << "Não foi possível abrir o ficheiro: " << filepath << endl;
                } else {
                    std::cout << "A executar comandos do ficheiro: " << filepath << std::endl;
                    string line;
                    while (getline(file, line)) {
                        processPhase2Command(*this, line); // Executa cada comando do ficheiro
                        checkAndEndSimulation();
                    }
                }
            } else {
                processPhase2Command(*this, command);
                checkAndEndSimulation();
                if (command == "terminar") {
                    break; // Garante que saímos corretamente do loop
                }
            }
        }
    }
}

void Simulator::initializeCityCaravans() {
    int globalCaravanID = 1; // ID único para todas as caravanas

    for (auto& city : map.getCities()) {
        city.initializeCaravans(globalCaravanID); // Passa o contador global
    }
}


void Simulator::showCaravanStatus()  {
    std::cout << "Status das Caravanas no instante atual:" << std::endl;
    for (const auto& caravan : caravans) {
        caravan->status();
    }
}

void Simulator::advanceSimulation(int n) {
    for (int i = 0; i < n; ++i) {
        elapsedInstants++;
        std::cout << "Simulacao avancando instante " << (i + 1) << " de " << n << "." << std::endl;

        for (auto& caravan : caravans) {
            if (caravan->getType() == "Secret") {
                caravan->updateInvisibility();
            }

            if (caravan->isAuto()) {
                if (caravan->isActive()) {
                    if (caravan->getType() == "Trade") {
                        handleTradeCaravanAuto(caravan);
                    } else if (caravan->getType() == "Military") {
                        handleMilitaryCaravanAuto(caravan);
                    }
                } else {
                    if (caravan->getType() == "Secret" && caravan->getWater() <= 0) {
                        caravan->becomeObstacle(map);
                        removeCaravan(caravan);
                        continue;
                    }
                    handleCaravanWithoutCrew(caravan);
                }
            }
        }

        // 🛠️ Atualiza e verifica interações com itens
        for (auto& caravan : caravans) {
            checkCaravanForItem(caravan); // Verifica se a caravana apanha algum item
        }

        updateItems(); // Atualiza a duração dos itens

        if (elapsedInstants % 10 == 0) {
            spawnItem(); // Gera novos itens a cada 10 instantes
        }


        // Verificação automática
        if (shouldEndSimulation()) {
            endSimulation();
            return; // Sai imediatamente da simulação
        }
        showCaravanStatus();
    }
}


void Simulator::showPrices() const {
    cout << "Precos das mercadorias nas cidades (T):" << endl;
    cout << " - Preco de compra: 1 moeda por tonelada" << endl;
    cout << " - Preco de venda: 2 moedas por tonelada" << endl;
}

void Simulator::addCoins(int n) {
    wallet.addCoins(n); // Usa a funcionalidade da Wallet
    cout << "Moedas atualizadas. Novo saldo: " << wallet.getCoins() << endl;
}

void Simulator::endSimulation() {
    std::cout << "\n--- Fim da Simulacao ---\n";
    std::cout << "Instantes decorridos: " << elapsedInstants << std::endl;
    std::cout << "Combates vencidos: " << totalCombatsWon << std::endl;
    std::cout << "Moedas restantes: " << wallet.getCoins() << std::endl;
    std::cout << "-------------------------\n";
}

bool Simulator::shouldEndSimulation() {
    bool noCaravans = caravans.empty();
    bool insufficientFunds = wallet.getCoins() < 100;
    bool noAvailableCaravansInCities = true;

    // Verifica se existem caravanas disponíveis nas cidades
    for (const auto& city : map.getCities()) {
        for (const auto& caravan : city.getCaravans()) {
            if (!caravan->isOwned() && wallet.getCoins() >= 100) { // Se houver caravanas não compradas e dinheiro suficiente
                noAvailableCaravansInCities = false;
                break;
            }
        }
        if (!noAvailableCaravansInCities) {
            break;
        }
    }

    return noCaravans && insufficientFunds && noAvailableCaravansInCities;
}

void Simulator::checkAndEndSimulation() {
    if (shouldEndSimulation()) {
        std::cout << "\nMeios insuficientes para continuar...\n";
        endSimulation();
        exit(0); // Sai imediatamente
    }
}

void Simulator::spawnItem() {
    if (items.size() >= 5) return; // Máximo de 5 itens

    int row = rand() % map.getRows();
    int col = rand() % map.getCols();

    if (map.getCell(row, col) == '.') { // Posição deve estar livre
        ItemType type = static_cast<ItemType>(rand() % 5);
        int duration = 20; // Pega do ficheiro de configuração

        items.push_back(new Item(type, row, col, duration));
        map.setCell(row, col, 'I'); // Representa um item no mapa
        std::cout << "[Simulator] Novo item apareceu em (" << row << ", " << col << ").\n";
    }
}
void Simulator::updateItems() {
    for (auto it = items.begin(); it != items.end();) {
        (*it)->decreaseLifetime();

        if ((*it)->isExpired()) {
            map.setCell((*it)->getRow(), (*it)->getCol(), '.'); // Remove do mapa
            delete *it;
            it = items.erase(it);
        } else {
            ++it;
        }
    }
}
void Simulator::checkCaravanForItem(Caravan* caravan) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (abs((*it)->getRow() - caravan->getRow()) <= 1 &&
            abs((*it)->getCol() - caravan->getCol()) <= 1) {
            (*it)->applyEffect(caravan);
            map.setCell((*it)->getRow(), (*it)->getCol(), '.'); // Remove do mapa
            delete *it;
            items.erase(it);
            break;
            }
    }
}
