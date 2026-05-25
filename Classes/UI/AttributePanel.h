#ifndef __ATTRIBUTE_PANEL_H__
#define __ATTRIBUTE_PANEL_H__

#include "cocos2d.h"
#include "Models/PlayerData.h"

class AttributePanel : public cocos2d::Node {
public:
    static AttributePanel* create(const PlayerData& player);
    virtual bool init(const PlayerData& player);

    void updateData(const PlayerData& player);

private:
    cocos2d::Label* _atkLabel;
    cocos2d::Label* _defLabel;
    cocos2d::Label* _hpLabel;
};

#endif