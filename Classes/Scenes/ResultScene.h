#ifndef __RESULT_SCENE_H__
#define __RESULT_SCENE_H__

#include "cocos2d.h"
#include "Constants.h"
#include "Models/LevelData.h"

class ResultScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene(int levelId, int stars);
    virtual bool init();
    void setLevelResult(int levelId, int stars);
    CREATE_FUNC(ResultScene);

private:
    void setupResultContent();
    void playStarFlyAnimation();
    void showGrayStars();
    void showRewardPanel();
    void buildRewardList();
    void onCollectClicked();
    cocos2d::DrawNode* createStarShape(const cocos2d::Color4F& color);

    int _levelId = 0;
    int _stars = 0;
    cocos2d::Vector<cocos2d::Node*> _rewardNodes;
    cocos2d::Vector<cocos2d::DrawNode*> _starNodes;
};

#endif