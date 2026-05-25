#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"

class BattleScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene(int levelId);
    virtual bool init();
    void setLevelId(int id);
    CREATE_FUNC(BattleScene);

private:
    void updateBattle(float dt);
    void onBattleComplete();

    int _levelId = 0;
    float _elapsed;
    cocos2d::Label* _timerLabel = nullptr;
    cocos2d::ProgressTimer* _progressBar = nullptr;
    cocos2d::Label* _titleLabel = nullptr;
};

#endif