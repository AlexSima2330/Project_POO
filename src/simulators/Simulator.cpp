#include "Simulator.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include "BarbarianCaravan.h"
#include "Wallet.h"
#include "Map.h"
#include "Commands.h"
#include "Fight.h"
#include "Item.h"

using namespace std;

Simulator::Simulator() : map(0, 0),  bufferRows(0), bufferCols(0), buffer(0, 0), wallet(0) {
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
                        initializeCityCaravans();
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
                return;
            } else {
               cerr << "Comando invalido nesta fase. Use 'config <ficheiro>' ou 'sair'." << endl;
            }
        }

        // Fase 2
        while (true) {
            displayMap();

            if (shouldEndSimulation()) {
                endSimulation();
                return;
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
     << "  invisible <N>      - Modo invisivel para a caravana N secreta\n"
     << "  saves <nome>      - Guarda o estado atual do buffer com o nome dado\n"
     << "  loads <nome>      - Carrega um estado anteriormente guardado\n"
     << "  lists             - Lista os nomes dos estados guardados\n"
     << "  dels <nome>       - Apaga um estado guardado pelo nome\n"
     << "Digite o comando: ";

            if (!getline(cin, command)) {
                return;
            }

            if (command.find("exec") == 0) {
                std::istringstream iss(command);
                std::string cmd, filename;
                iss >> cmd >> filename;

                if (filename.empty()) {
                    filename = "run_commands.txt";
                }

                std::string filepath = "./config/" + filename;

                ifstream file(filepath);
                if (!file.is_open()) {
                    cerr << "Não foi possível abrir o ficheiro: " << filepath << endl;
                } else {
                    std::cout << "A executar comandos do ficheiro: " << filepath << std::endl;
                    string line;
                    while (getline(file, line)) {
                        processPhase2Command(*this, line);
                        checkAndEndSimulation();
                    }
                }
            } else {
                processPhase2Command(*this, command);
                checkAndEndSimulation();
                if (command == "terminar") {
                    break;
                }
            }
        }
    }
}

void Simulator::initializeCityCaravans() {
    int globalCaravanID = 1;

    for (auto& city : map.getCities()) {
        city.initializeCaravans(globalCaravanID);
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
            if (!caravan->isActive()) {
                handleCaravanWithoutCrew(caravan);
                continue;
            }

            if (caravan->getType() == "Secret") {
                caravan->updateInvisibility();
            }

            if (caravan->isAuto()) {
                if (caravan->getType() == "Trade") {
                    handleTradeCaravanAuto(caravan);
                } else if (caravan->getType() == "Military") {
                    handleMilitaryCaravanAuto(caravan);
                } else if (caravan->getType() == "Secret") {
                    handleSecretCaravanAuto(caravan);
                }
            }

            if (caravan->getType() == "Barbarian") {
                handleBarbarianCaravanAuto(static_cast<BarbarianCaravan*>(caravan));
            }
        }

        for (auto& caravan : caravans) {
            checkCaravanForItem(caravan);
        }

        updateItems();

        if (elapsedInstants % timeBetweenItems == 0) {
            spawnItem();
        }

        if (elapsedInstants % timeBetweenBarbarians == 0) {
            spawnBarbarianCaravan();
        }

        resolveCombats();

        if (shouldEndSimulation()) {
            endSimulation();
            return;
        }
    }
}

void Simulator::showPrices() const {
    cout << "Precos nas cidades (T):" << endl;
    cout << "preco_compra_mercadoria 1" << endl;
    cout << "preco_venda_mercadoria 2" << endl;
    cout << "preco_caravana 100" << endl;
}

void Simulator::addCoins(int n) {
    wallet.addCoins(n);
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

    // Verifica as caravanas existentes nas cidades
    for (const auto& city : map.getCities()) {
        for (const auto& caravan : city.getCaravans()) {
            if (!caravan->isOwned() && wallet.getCoins() >= 100) {
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
        exit(0);
    }
}

void Simulator::spawnItem() {
    if (items.size() >= maxItems) return;

    int row = rand() % map.getRows();
    int col = rand() % map.getCols();

    if (map.getCell(row, col) == '.') {
        ItemType type = static_cast<ItemType>(rand() % 5);

        int duration = itemDuration;

        items.push_back(new Item(type, row, col, duration));
        map.setCell(row, col, 'I');
        std::cout << "[Item] Novo item apareceu em (" << row << ", " << col << ")" << "\n";
    }
}

void Simulator::updateItems() {
    for (auto it = items.begin(); it != items.end();) {
        (*it)->decreaseLifetime();

        if ((*it)->isExpired()) {
            map.setCell((*it)->getRow(), (*it)->getCol(), '.');
            delete *it;
            it = items.erase(it);
        } else {
            ++it;
        }
    }
}
void Simulator::checkCaravanForItem(Caravan* caravan) {
    int row = caravan->getRow();
    int col = caravan->getCol();

    std::vector<std::pair<int, int>> adjacentPositions = {
        {row - 1, col},
        {row + 1, col},
        {row, col - 1},
        {row, col + 1}
    };

    for (auto& [adjRow, adjCol] : adjacentPositions) {
        auto [wrappedRow, wrappedCol] = map.wrapCoordinates(adjRow, adjCol);

        for (auto it = items.begin(); it != items.end(); ++it) {
            if ((*it)->getRow() == wrappedRow && (*it)->getCol() == wrappedCol) {
                (*it)->applyEffect(caravan, this);

                map.setCell(wrappedRow, wrappedCol, '.');

                delete *it;
                items.erase(it);

                std::cout << "[Item] Caravana " << caravan->getId()
                          << " apanhou um item na posicao (" << wrappedRow
                          << ", " << wrappedCol << ").\n";
                return;
            }
        }
    }
}

const Wallet& Simulator::getWallet() const {
    return wallet;
}

Wallet& Simulator::getWallet() {
    return wallet;
}

void Simulator::removeCaravanMine(Caravan* caravan) {
    for (auto it = caravans.begin(); it != caravans.end(); ++it) {
        if (*it == caravan) {
            int row = caravan->getRow();
            int col = caravan->getCol();

            map.setCell(row, col, '.');

            delete *it;
            caravans.erase(it);

            std::cout << "[Simulator] Caravana removida da posicao (" << row << ", " << col << ").\n";
            return;
        }
    }

    std::cout << "[Erro] Tentativa de remover uma caravana que nao existe.\n";
}

void Simulator::resolveCombats() {
    for (auto& caravan : caravans) {
        if (!caravan->isActive()) continue;

        for (auto& other : caravans) {
            if (caravan == other || !other->isActive()) continue;

            bool isAdjacent =
                (std::abs(caravan->getRow() - other->getRow()) == 1 && caravan->getCol() == other->getCol()) ||
                (std::abs(caravan->getCol() - other->getCol()) == 1 && caravan->getRow() == other->getRow());

            if (isAdjacent) {
                if (caravan->isOwned() && other->getType() == "Barbarian") {
                    std::cout << "[Combate] Caravana ID " << caravan->getId()
                              << " encontrou uma caravana barbara !" << std::endl;

                    Fight::handleCombat(caravan, other, this);
                }
            }
        }
    }
}