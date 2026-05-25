#ifndef __LEVEL_DATA_H__
#define __LEVEL_DATA_H__

#include "Constants.h"
#include <string>
#include <vector>
#include "Models/ItemData.h"

struct LevelData {
    int id;
    std::string name;
    LevelState state;
    int stars;
    std::vector<std::pair<ItemData, int>> rewards;

    LevelData() : id(0), state(LevelState::LOCKED), stars(0) {}
};

#endif