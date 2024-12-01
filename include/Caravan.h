#ifndef CARAVAN_H
#define CARAVAN_H

#include <string>
#include <iostream>

// Classe base para Caravanas
class Caravan {
protected:
    int id;              // Identificador único da caravana
    int crew;            // Número de membros da tripulação
    int cargo;           // Carga atual
    int water;           // Quantidade de água disponível
    int maxCargo;        // Capacidade máxima de carga
    int maxWater;        // Capacidade máxima de água
    int row;             // Linha (posição atual no mapa)
    int col;             // Coluna (posição atual no mapa)
    std::string type;    // Tipo da caravana (ex.: Comércio, Militar)

public:
    // Construtor
    Caravan(int id, const std::string &type, int maxCargo, int maxWater, int row = -1, int col = -1);

    // Métodos básicos
    int getId() const { return id; }
    int getRow() const { return row; }
    int getCol() const { return col; }
    void setPosition(int newRow, int newCol);

    // Movimento
    virtual void move(char direction); // Mover-se numa direção ('C', 'B', 'D', 'E')

    // Status
    virtual void status() const; // Exibir o estado da caravana

    // Destrutor virtual
    virtual ~Caravan() = default;

    void addResource(); // Método para coletar recursos
};

// Classe TradeCaravan (Caravana de Comércio)
class TradeCaravan : public Caravan {
public:
    // Construtor
    TradeCaravan(int id);

    // Movimento especializado
    void move(char direction) override;

    // Status especializado
    void status() const override;
};

// Classe MilitaryCaravan (Caravana Militar)
class MilitaryCaravan : public Caravan {
public:
    // Construtor
    MilitaryCaravan(int id);

    // Movimento especializado
    void move(char direction) override;

    // Status especializado
    void status() const override;
};

#endif
