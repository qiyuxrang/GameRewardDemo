#ifndef __INVENTORY_BAR_LAYER_H__
#define __INVENTORY_BAR_LAYER_H__

#include "cocos2d.h"

class InventoryBarLayer : public cocos2d::Layer {
public:
    static InventoryBarLayer* create();
    virtual bool init();
    void refreshSlots();

private:
    cocos2d::Vector<cocos2d::Sprite*> _slotSprites;
    cocos2d::Vector<cocos2d::Label*> _countLabels;
};

#endif