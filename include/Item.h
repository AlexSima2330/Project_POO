#ifndef ITEM_H
#define ITEM_H

#include "Caravan.h"
#include <string>

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
    int remainingTurns; // Duração do item no mapa

public:
    Item(ItemType type, int row, int col, int duration);

    int getRow() const;
    int getCol() const;
    ItemType getType() const;
    void decreaseLifetime();
    bool isExpired() const;
    void applyEffect(Caravan* caravan);
};

#endif