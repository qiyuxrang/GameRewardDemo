#ifndef __LEVEL_UP_EFFECT_LAYER_H__
#define __LEVEL_UP_EFFECT_LAYER_H__

#include "cocos2d.h"

class LevelUpEffectLayer : public cocos2d::Layer {
public:
    static LevelUpEffectLayer* create(int newLevel);
    virtual bool init(int newLevel);
    void setOnCompleteCallback(const std::function<void()>& callback);

private:
    void playFlashEffect();
    void playDarkOverlay();
    void playTextEffect();
    void playAttributeEffect();
    void playParticleEffect();
    void playComplete();

    int _newLevel;
    std::function<void()> _onComplete;
};

#endif