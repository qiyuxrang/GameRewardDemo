#ifndef __ICON_FACTORY_H__
#define __ICON_FACTORY_H__

#include "cocos2d.h"
#include "Constants.h"

class IconFactory {
public:
    // Create a recognizable icon for each item type
    static cocos2d::Node* createItemIcon(ItemType type, float size = 50.0f);

private:
    static cocos2d::Color3B parseHexColor(const std::string& hex);
};

#endif