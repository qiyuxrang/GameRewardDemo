#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include "Models/PlayerData.h"
#include "Models/LevelData.h"
#include "Models/ItemData.h"
#include <vector>

class DataManager {
public:
    static DataManager* getInstance();

    PlayerData& getPlayer();
    std::vector<LevelData>& getLevels();
    LevelData* getLevel(int id);
    const ItemData& getItemDef(int itemId);

    void completeLevel(int levelId, int stars);
    void reset();

private:
    DataManager();
    void initLevels();

    static DataManager* _instance;
    PlayerData _player;
    std::vector<LevelData> _levels;
};

#endif