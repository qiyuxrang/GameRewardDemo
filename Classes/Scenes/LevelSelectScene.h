#ifndef __LEVEL_SELECT_SCENE_H__
#define __LEVEL_SELECT_SCENE_H__

#include "cocos2d.h"
#include "Constants.h"

class LevelSelectScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(LevelSelectScene);

private:
    void buildLevelNodes();
    void onLevelClicked(int levelId);
    void updatePlayerInfo();
    void drawStarShape(cocos2d::DrawNode* draw, const cocos2d::Vec2& center, float size, const cocos2d::Color4F& color);

    cocos2d::Label* _levelLabel;
    cocos2d::Label* _expLabel;
    cocos2d::Label* _goldLabel;
    cocos2d::ProgressTimer* _expBar;
};

#endif