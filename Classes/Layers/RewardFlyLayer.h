#ifndef __REWARD_FLY_LAYER_H__
#define __REWARD_FLY_LAYER_H__

#include "cocos2d.h"
#include "Models/ItemData.h"
#include <vector>
#include <functional>

class RewardFlyLayer : public cocos2d::Layer {
public:
    static RewardFlyLayer* create(const std::vector<std::pair<ItemData, int>>& rewards,
                                  const std::vector<cocos2d::Vec2>& startPositions);
    virtual bool init(const std::vector<std::pair<ItemData, int>>& rewards,
                      const std::vector<cocos2d::Vec2>& startPositions);

    void setOnCompleteCallback(const std::function<void()>& callback);

private:
    void startFlyAnimation();
    cocos2d::Vec2 getInventoryTargetPos(int slotIndex);

    std::vector<std::pair<ItemData, int>> _rewards;
    std::vector<cocos2d::Vec2> _startPositions;
    std::function<void()> _onComplete;
    int _flyCount;
    int _flyDone;
};

#endif