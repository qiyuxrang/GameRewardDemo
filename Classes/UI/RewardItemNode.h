#ifndef __REWARD_ITEM_NODE_H__
#define __REWARD_ITEM_NODE_H__

#include "cocos2d.h"
#include "Models/ItemData.h"

class RewardItemNode : public cocos2d::Node {
public:
    static RewardItemNode* create(const ItemData& itemData, int count);
    virtual bool init(const ItemData& itemData, int count);

    cocos2d::Sprite* getIconSprite() const;
    const ItemData& getItemData() const;
    int getCount() const;

private:
    ItemData _itemData;
    int _count;
    cocos2d::Sprite* _iconSprite;
    cocos2d::Label* _nameLabel;
    cocos2d::Label* _countLabel;
};

#endif