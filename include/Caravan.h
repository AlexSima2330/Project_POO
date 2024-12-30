#ifndef CARAVAN_H
#define CARAVAN_H

#include <string>
#include "Map.h"

class Map;

class Caravan {
protected:
    int id;
    int crew;
    int cargo, maxCargo;
    int water, maxWater;
    int row, col;
    std::string type;
    bool onCharger = false;
    bool inCity = false;
    int movesThisTurn;
    bool isAutoManaged = false;
    int autoTurnsWithoutCrew = 0;
    bool owned = false;
    bool isInvisible = false;
    int invisibleTurns = 0;
    std::string lastDirection;

public:
    Caravan(int id, const std::string &type, int row = -1, int col = -1);

    int getId() const { return id; }
    int getRow() const { return row; }
    int getCol() const { return col; }
    std::string getType() const { return type; }
    void setPosition(int newRow, int newCol);

    virtual void move(const std::string& direction);

    virtual void status() const;

    bool processMovement(Map &map);
    void becomeObstacle(Map &map);

    void addResource();

    int getWater() const { return water; }

    virtual int consumeWater() const = 0;

    void setWater(int amount) {
        if (amount >= 0 && amount <= maxWater) {
            water = amount;
        }
    }

    void refillWater() {
        water = maxWater;
    }

    bool isActive() const {
        return crew > 0;
    }

    void loseCrew(int amount) {
        if (crew > 0) {
            crew -= amount;
            if (crew < 0) crew = 0;
        }
    }

    int getCrew() const {
        return crew;
    }
    void setCrew(int newCrew) {
        crew = std::max(0, newCrew);
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

    bool addCargo(int quantity);
    bool removeCargo(int quantity);

    virtual int getMaxMovesPerTurn() const = 0;

    bool isAuto() const { return isAutoManaged; }
    void setAuto(bool autoState) { isAutoManaged = autoState; }
    void incrementAutoTurnsWithoutCrew() { autoTurnsWithoutCrew++; }
    void resetAutoTurnsWithoutCrew() { autoTurnsWithoutCrew = 0; }
    int getAutoTurnsWithoutCrew() const { return autoTurnsWithoutCrew; }

    bool isOwned() const { return owned; }
    void setOwned(bool value) { owned = value; }
    void activateInvisibility();
    void updateInvisibility();
    bool isCurrentlyInvisible() const { return isInvisible; }

    void setLastDirection(const std::string& direction) { lastDirection = direction; }
    std::string getLastDirection() const { return lastDirection; }
    void moveRandomly(Map& map);
    void moveInLastDirection(Map& map);

    virtual ~Caravan() = default;

};

class TradeCaravan : public Caravan {
public:
    TradeCaravan(int id, int initialCrew);

    void move(const std::string& direction) override;

    int consumeWater() const override;

    int getMaxMovesPerTurn() const override { return 2; }

};

class MilitaryCaravan : public Caravan {
public:
    MilitaryCaravan(int id, int initialCrew);

    void move(const std::string& direction) override;

    int consumeWater() const override;

    int getMaxMovesPerTurn() const override { return 3; }
};

class SecretCaravan : public Caravan {
public:
    SecretCaravan(int id, int initialCrew);

    void move(const std::string& direction) override;

    int consumeWater() const override;

    int getMaxMovesPerTurn() const override { return 3; }

    void handleNoWater(Map& map);
};

#endif