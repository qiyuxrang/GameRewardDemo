#include "Managers/DataManager.h"

const ItemData ITEM_GOLD_COIN(1, "Gold", ItemType::GOLD, "", "#FFD700");
const ItemData ITEM_EXP_ORB(2, "Exp Orb", ItemType::EXP_ORB, "", "#64FF64");
const ItemData ITEM_SWORD(3, "Iron Sword", ItemType::WEAPON, "", "#AAAACC");
const ItemData ITEM_SHIELD(4, "Wood Shield", ItemType::ARMOR, "", "#8B4513");
const ItemData ITEM_HP_POTION(5, "HP Potion", ItemType::POTION, "", "#FF6464");
const ItemData ITEM_KEY_ITEM(6, "Key Item", ItemType::KEY, "", "#FFD700");

DataManager* DataManager::_instance = nullptr;

DataManager* DataManager::getInstance() {
    if (!_instance) {
        _instance = new DataManager();
        _instance->initLevels();
    }
    return _instance;
}

DataManager::DataManager() {}

void DataManager::initLevels() {
    LevelData level1;
    level1.id = 1;
    level1.name = "Trial";
    level1.state = LevelState::AVAILABLE;
    level1.stars = 0;
    level1.rewards = {
        {ITEM_GOLD_COIN, 50},
        {ITEM_EXP_ORB, 30}
    };
    _levels.push_back(level1);

    LevelData level2;
    level2.id = 2;
    level2.name = "Dark Forest";
    level2.state = LevelState::LOCKED;
    level2.stars = 0;
    level2.rewards = {
        {ITEM_GOLD_COIN, 80},
        {ITEM_EXP_ORB, 50},
        {ITEM_HP_POTION, 2}
    };
    _levels.push_back(level2);

    LevelData level3;
    level3.id = 3;
    level3.name = "Dragon Nest";
    level3.state = LevelState::LOCKED;
    level3.stars = 0;
    level3.rewards = {
        {ITEM_GOLD_COIN, 120},
        {ITEM_EXP_ORB, 80},
        {ITEM_SWORD, 1}
    };
    _levels.push_back(level3);

    LevelData level4;
    level4.id = 4;
    level4.name = "Abyss Gate";
    level4.state = LevelState::LOCKED;
    level4.stars = 0;
    level4.rewards = {
        {ITEM_GOLD_COIN, 200},
        {ITEM_EXP_ORB, 100},
        {ITEM_SHIELD, 1},
        {ITEM_HP_POTION, 3}
    };
    _levels.push_back(level4);

    LevelData level5;
    level5.id = 5;
    level5.name = "Final Trial";
    level5.state = LevelState::LOCKED;
    level5.stars = 0;
    level5.rewards = {
        {ITEM_GOLD_COIN, 300},
        {ITEM_EXP_ORB, 150},
        {ITEM_KEY_ITEM, 1}
    };
    _levels.push_back(level5);
}

PlayerData& DataManager::getPlayer() {
    return _player;
}

std::vector<LevelData>& DataManager::getLevels() {
    return _levels;
}

LevelData* DataManager::getLevel(int id) {
    for (auto& level : _levels) {
        if (level.id == id) return &level;
    }
    return nullptr;
}

const ItemData& DataManager::getItemDef(int itemId) {
    static ItemData unknown(0, "Unknown", ItemType::GOLD, "", "#888888");
    switch (itemId) {
        case 1: return ITEM_GOLD_COIN;
        case 2: return ITEM_EXP_ORB;
        case 3: return ITEM_SWORD;
        case 4: return ITEM_SHIELD;
        case 5: return ITEM_HP_POTION;
        case 6: return ITEM_KEY_ITEM;
        default: return unknown;
    }
}

void DataManager::completeLevel(int levelId, int stars) {
    auto* level = getLevel(levelId);
    if (!level) return;

    level->state = LevelState::CLEARED;
    if (stars > level->stars) level->stars = stars;

    int nextId = levelId + 1;
    auto* nextLevel = getLevel(nextId);
    if (nextLevel && nextLevel->state == LevelState::LOCKED) {
        nextLevel->state = LevelState::AVAILABLE;
    }

    for (auto& reward : level->rewards) {
        if (reward.first.type == ItemType::GOLD) {
            _player.gold += reward.second;
        } else if (reward.first.type == ItemType::EXP_ORB) {
            _player.addExp(reward.second);
        } else {
            _player.addItem(reward.first.id, reward.second);
        }
    }
}

void DataManager::reset() {
    _player = PlayerData();
    _levels.clear();
    initLevels();
}