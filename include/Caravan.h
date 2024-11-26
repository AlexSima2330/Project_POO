#ifndef CARAVAN_H
#define CARAVAN_H

#include <string>

class Caravan {
protected:
    int id;
    int crew;
    int cargo;
    int water;
    int maxCargo;
    int maxWater;
    std::string type;

public:
    Caravan(int id, const std::string &type, int maxCargo, int maxWater);
    virtual void move(const std::string &direction);
    virtual void status() const;
    virtual ~Caravan() = default;
};

class TradeCaravan : public Caravan {
public:
    TradeCaravan(int id);
    void move(const std::string &direction) override;
};

class MilitaryCaravan : public Caravan {
public:
    MilitaryCaravan(int id);
    void move(const std::string &direction) override;
};

#endif
