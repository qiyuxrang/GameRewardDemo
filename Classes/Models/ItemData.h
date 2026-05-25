#ifndef __ITEM_DATA_H__
#define __ITEM_DATA_H__

#include "Constants.h"
#include <string>

struct ItemData {
    int id;
    std::string name;
    ItemType type;
    std::string iconPath;
    std::string colorHex;

    ItemData() : id(0), name(""), type(ItemType::GOLD), iconPath(""), colorHex("") {}
    ItemData(int id, const std::string& name, ItemType type,
             const std::string& iconPath, const std::string& colorHex)
        : id(id), name(name), type(type), iconPath(iconPath), colorHex(colorHex) {}
};

extern const ItemData ITEM_GOLD_COIN;
extern const ItemData ITEM_EXP_ORB;
extern const ItemData ITEM_SWORD;
extern const ItemData ITEM_SHIELD;
extern const ItemData ITEM_HP_POTION;
extern const ItemData ITEM_KEY_ITEM;

#endif