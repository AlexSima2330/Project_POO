#ifndef ITEM_H
#define ITEM_H

#include "Caravan.h" // Garantir que Caravan está incluído

// Forward Declaration
class Simulator;

enum class ItemType {
    PandoraBox,
    TreasureChest,
    Cage,
    Mine,
    Surprise
};

class Item {
private:
    ItemType type;
    int row, col;
    int remainingTurns;

public:
    Item(ItemType type, int row, int col, int duration);

    ItemType getType() const;
    int getRow() const;
    int getCol() const;
    void decreaseLifetime();
    bool isExpired() const;

    // Agora com forward declaration
    void applyEffect(Caravan* caravan, Simulator* simulator);
};

#endif