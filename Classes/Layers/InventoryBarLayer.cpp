#include "Layers/InventoryBarLayer.h"
#include "Managers/DataManager.h"
#include "UI/IconFactory.h"
#include "Constants.h"

USING_NS_CC;

InventoryBarLayer* InventoryBarLayer::create() {
    auto* layer = new InventoryBarLayer();
    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    }
    delete layer;
    return nullptr;
}

bool InventoryBarLayer::init() {
    if (!Layer::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto barBg = Sprite::create();
    barBg->setTextureRect(Rect(0, 0, visibleSize.width, 100));
    barBg->setColor(Color3B(40, 40, 60));
    barBg->setOpacity(200);
    barBg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 50));
    this->addChild(barBg);

    float slotWidth = 80;
    float totalWidth = INVENTORY_MAX_SLOTS * slotWidth;
    float startX = origin.x + (visibleSize.width - totalWidth) / 2;

    for (int i = 0; i < INVENTORY_MAX_SLOTS; i++) {
        float xPos = startX + i * slotWidth + slotWidth / 2;
        float yPos = origin.y + 50;

        auto slotBg = Sprite::create();
        slotBg->setTextureRect(Rect(0, 0, 70, 70));
        slotBg->setColor(Color3B(60, 60, 80));
        slotBg->setPosition(Vec2(xPos, yPos));
        this->addChild(slotBg);

        auto slotSprite = Sprite::create();
        slotSprite->setTextureRect(Rect(0, 0, 50, 50));
        slotSprite->setColor(Color3B(100, 100, 100));
        slotSprite->setOpacity(0);
        slotSprite->setPosition(Vec2(xPos, yPos + 5));
        this->addChild(slotSprite);
        _slotSprites.pushBack(slotSprite);

        // Icon node placeholder (will be replaced in refreshSlots)
        auto iconNode = Node::create();
        iconNode->setPosition(Vec2(xPos, yPos + 5));
        iconNode->setVisible(false);
        this->addChild(iconNode, 2, 200 + i);  // tag 200+i for icon nodes

        auto countLabel = Label::createWithTTF("", "fonts/arial.ttf", 16);
        countLabel->setPosition(Vec2(xPos, yPos - 25));
        countLabel->setColor(colorGold());
        this->addChild(countLabel);
        _countLabels.pushBack(countLabel);
    }

    refreshSlots();
    return true;
}

void InventoryBarLayer::refreshSlots() {
    auto& inventory = DataManager::getInstance()->getPlayer().inventory;
    int slotIndex = 0;

    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (slotIndex >= INVENTORY_MAX_SLOTS) break;

        int itemId = it->first;
        int count = it->second;
        auto& itemDef = DataManager::getInstance()->getItemDef(itemId);
        auto slotSprite = _slotSprites.at(slotIndex);
        auto countLabel = _countLabels.at(slotIndex);

        // Hide placeholder sprite, show real icon
        slotSprite->setOpacity(0);

        // Replace icon node with real icon from IconFactory
        auto oldIcon = this->getChildByTag(200 + slotIndex);
        if (oldIcon) oldIcon->removeFromParent();

        auto iconNode = IconFactory::createItemIcon(itemDef.type, 40.0f);
        float slotWidth = 80;
        float totalWidth = INVENTORY_MAX_SLOTS * slotWidth;
        float startX = (Director::getInstance()->getVisibleSize().width - totalWidth) / 2;
        float xPos = startX + slotIndex * slotWidth + slotWidth / 2;
        float yPos = 55;
        iconNode->setPosition(Vec2(xPos, yPos));
        iconNode->setVisible(true);
        this->addChild(iconNode, 2, 200 + slotIndex);

        countLabel->setString(std::to_string(count));
        countLabel->runAction(Sequence::create(
            ScaleTo::create(NUM_POP_DURATION, 1.5f),
            ScaleTo::create(NUM_POP_DURATION, 1.0f),
            nullptr
        ));

        slotIndex++;
    }

    // Clear remaining slots
    for (; slotIndex < INVENTORY_MAX_SLOTS; slotIndex++) {
        _slotSprites.at(slotIndex)->setOpacity(0);
        _countLabels.at(slotIndex)->setString("");

        auto oldIcon = this->getChildByTag(200 + slotIndex);
        if (oldIcon) oldIcon->removeFromParent();

        auto emptyNode = Node::create();
        emptyNode->setVisible(false);
        this->addChild(emptyNode, 2, 200 + slotIndex);
    }
}