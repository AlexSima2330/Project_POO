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

public:
    // Construtor
    Caravan(int id, const std::string &type, int row = -1, int col = -1);

    // Métodos básicos
    int getId() const { return id; }
    int getRow() const { return row; }
    int getCol() const { return col; }
    void setPosition(int newRow, int newCol);

    // Movimento
    virtual void move(const std::string& direction); // Mover-se numa direção ('C', 'B', 'D', 'E')

    // Status
    virtual void status() const; // Exibir o estado da caravana

    // Processamento do movimento e penalidades
    bool processMovement(Map &map, int waterConsumption);

    // Converte a caravana para um obstáculo no mapa
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

    bool wasOnCharger() const {
        return onCharger;
    }

    void setOnCharger(bool value) {
        onCharger = value;
    }

    bool isInCity() const { return inCity; }
    void setInCity(bool value) { inCity = value; }

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

};

// Classe MilitaryCaravan (Caravana Militar)
class MilitaryCaravan : public Caravan {
public:
    // Construtor
    MilitaryCaravan(int id, int initialCrew);

    // Movimento especializado
    void move(const std::string& direction) override;

    int consumeWater() const override;

};

#endif
