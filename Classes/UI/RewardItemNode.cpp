#include "UI/RewardItemNode.h"
#include "UI/IconFactory.h"
#include "Constants.h"

USING_NS_CC;

RewardItemNode* RewardItemNode::create(const ItemData& itemData, int count) {
    auto* node = new RewardItemNode();
    if (node && node->init(itemData, count)) {
        node->autorelease();
        return node;
    }
    delete node;
    return nullptr;
}

bool RewardItemNode::init(const ItemData& itemData, int count) {
    if (!Node::init()) return false;

    _itemData = itemData;
    _count = count;

    // Use IconFactory for recognizable shapes
    _iconSprite = Sprite::create();
    _iconSprite->setTextureRect(Rect(0, 0, 50, 50));
    _iconSprite->setColor(Color3B(200, 200, 200));
    _iconSprite->setOpacity(0);  // Hide placeholder, icon drawn below
    _iconSprite->setPosition(Vec2(-80, 0));
    this->addChild(_iconSprite);

    auto iconNode = IconFactory::createItemIcon(itemData.type, 50.0f);
    iconNode->setPosition(Vec2(-80, 0));
    this->addChild(iconNode);

    _nameLabel = Label::createWithTTF(itemData.name, "fonts/arial.ttf", 24);
    _nameLabel->setPosition(Vec2(0, 0));
    _nameLabel->setColor(colorNormal());
    this->addChild(_nameLabel);

    _countLabel = Label::createWithTTF("x" + std::to_string(count), "fonts/arial.ttf", 24);
    _countLabel->setPosition(Vec2(80, 0));
    _countLabel->setColor(colorGold());
    this->addChild(_countLabel);

    return true;
}

Sprite* RewardItemNode::getIconSprite() const {
    return _iconSprite;
}

const ItemData& RewardItemNode::getItemData() const {
    return _itemData;
}

int RewardItemNode::getCount() const {
    return _count;
}