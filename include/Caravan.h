#ifndef CARAVAN_H
#define CARAVAN_H

#include <string>
#include <iostream>
#include "Map.h" // Inclui para manipular o mapa diretamente

class Map;

// Classe base para Caravanas
class Caravan {
protected:
    int id;              // Identificador único da caravana
    int crew;            // Número de membros da tripulação
    int cargo, maxCargo;          // Carga atual
    int water, maxWater;           // Quantidade de água disponível
    int row, col;             // Linha (posição atual no mapa)// Coluna (posição atual no mapa)
    std::string type;
    bool onCharger = false;
    bool inCity = false;
    int movesThisTurn;
    bool isAutoManaged = false; // Indica se a caravana está em modo automático
    int autoTurnsWithoutCrew = 0; // Contador para caravana sem tripulantes
    bool owned = false;
    bool isInvisible = false; // Estado de invisibilidade
    int invisibleTurns = 0;   // Contador de turnos de invisibilidade

public:
    // Construtor
    Caravan(int id, const std::string &type, int row = -1, int col = -1);

    // Métodos básicos
    int getId() const { return id; }
    int getRow() const { return row; }
    int getCol() const { return col; }
    std::string getType() const { return type; }
    void setPosition(int newRow, int newCol);

    // Movimento
    virtual void move(const std::string& direction); // Mover-se numa direção ('C', 'B', 'D', 'E')

    // Status
    virtual void status() const; // Exibir o estado da caravana

    // Processamento do movimento e penalidades
    bool processMovement(Map &map);
    void becomeObstacle(Map &map);

    void addResource(); // Coletar recursos

    int getWater() const { return water; }

    virtual int consumeWater() const = 0;

    void setWater(int amount) {
        if (amount >= 0 && amount <= maxWater) {
            water = amount;
        }
    }

    // Reabastecer água em carregador
    void refillWater() {
        water = maxWater;
    }

    // Verifica se a caravana ainda está ativa (tem tripulantes)
    bool isActive() const {
        return crew > 0;
    }

    // Reduz o número de tripulantes
    void loseCrew(int amount) {
        if (crew > 0) {
            crew -= amount;
            if (crew < 0) crew = 0; // Evita números negativos
            std::cout << "[Caravana] ID: " << id << " perdeu " << amount << " tripulantes. Restantes: " << crew << std::endl;
        }
    }

    // Retorna o número de tripulantes restantes
    int getCrew() const {
        return crew;
    }
    void setCrew(int newCrew) {
        crew = std::max(0, newCrew); // Garante que o número de tripulantes nunca seja negativo
    }

    bool wasOnCharger() const {
        return onCharger;
    }

    void setOnCharger(bool value) {
        onCharger = value;
    }

    bool isInCity() const { return inCity; }
    void setInCity(bool value) { inCity = value; }

    void resetMoves() {
        movesThisTurn = 0;
    }

    void incrementMove() {
        movesThisTurn++;
    }

    bool canMove() const {
        return movesThisTurn < getMaxMovesPerTurn();
    }

    int getCargo() const { return cargo; }
    int getMaxCargo() const { return maxCargo; }
    void setCargo(int newCargo) { cargo = newCargo; }

    bool addCargo(int quantity); // Adiciona uma quantidade de carga, respeitando o limite máximo.
    bool removeCargo(int quantity); // Remove uma quantidade de carga, sem permitir valores negativos.


    virtual int getMaxMovesPerTurn() const = 0;

    bool isAuto() const { return isAutoManaged; }
    void setAuto(bool autoState) { isAutoManaged = autoState; }
    void incrementAutoTurnsWithoutCrew() { autoTurnsWithoutCrew++; }
    void resetAutoTurnsWithoutCrew() { autoTurnsWithoutCrew = 0; }
    int getAutoTurnsWithoutCrew() const { return autoTurnsWithoutCrew; }

    bool isOwned() const { return owned; }
    void setOwned(bool value) { owned = value; }
    void activateInvisibility(); // Ativa o modo invisível
    void updateInvisibility();   // Atualiza a duração da invisibilidade
    bool isCurrentlyInvisible() const { return isInvisible; }

    virtual ~Caravan() = default;

};

// Classe TradeCaravan (Caravana de Comércio)
class TradeCaravan : public Caravan {
public:
    // Construtor
    TradeCaravan(int id, int initialCrew);

    // Movimento especializado
    void move(const std::string& direction) override;

    int consumeWater() const override;

    int getMaxMovesPerTurn() const override { return 2; }

};

// Classe MilitaryCaravan (Caravana Militar)
class MilitaryCaravan : public Caravan {
public:
    // Construtor
    MilitaryCaravan(int id, int initialCrew);

    // Movimento especializado
    void move(const std::string& direction) override;

    int consumeWater() const override;

    int getMaxMovesPerTurn() const override { return 3; }

};

class SecretCaravan : public Caravan {
public:
    // Construtor
    SecretCaravan(int id, int initialCrew);

    // Movimento especializado
    void move(const std::string& direction) override;

    int consumeWater() const override;

    int getMaxMovesPerTurn() const override { return 3; }

    void handleNoWater(Map& map);
};

#endif