#ifndef __PLAYER_DATA_H__
#define __PLAYER_DATA_H__

#include <map>
#include "Constants.h"
#include "Models/ItemData.h"

struct PlayerData {
    int level;
    int exp;
    int gold;
    int attack;
    int defense;
    int hp;

    std::map<int, int> inventory;

    PlayerData() : level(1), exp(0), gold(0), attack(10), defense(5), hp(100) {}

    bool canLevelUp() const {
        return exp >= EXP_PER_LEVEL;
    }

    void addExp(int amount) {
        exp += amount;
    }

    void levelUp() {
        level++;
        exp -= EXP_PER_LEVEL;
        attack += 2;
        defense += 1;
        hp += 20;
    }

    void addItem(int itemId, int count) {
        inventory[itemId] += count;
    }

    bool hasItem(int itemId) const {
        auto it = inventory.find(itemId);
        return it != inventory.end() && it->second > 0;
    }
};

#endif