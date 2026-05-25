#include "UI/AttributePanel.h"
#include "Constants.h"

USING_NS_CC;

AttributePanel* AttributePanel::create(const PlayerData& player) {
    auto* panel = new AttributePanel();
    if (panel && panel->init(player)) {
        panel->autorelease();
        return panel;
    }
    delete panel;
    return nullptr;
}

bool AttributePanel::init(const PlayerData& player) {
    if (!Node::init()) return false;

    auto bg = Sprite::create();
    bg->setTextureRect(Rect(0, 0, 200, 120));
    bg->setColor(Color3B(50, 50, 70));
    bg->setOpacity(180);
    bg->setPosition(Vec2(0, 0));
    this->addChild(bg);

    _atkLabel = Label::createWithTTF("", "fonts/arial.ttf", 20);
    _atkLabel->setPosition(Vec2(0, 30));
    _atkLabel->setColor(Color3B(255, 100, 100));
    this->addChild(_atkLabel);

    _defLabel = Label::createWithTTF("", "fonts/arial.ttf", 20);
    _defLabel->setPosition(Vec2(0, 0));
    _defLabel->setColor(Color3B(100, 100, 255));
    this->addChild(_defLabel);

    _hpLabel = Label::createWithTTF("", "fonts/arial.ttf", 20);
    _hpLabel->setPosition(Vec2(0, -30));
    _hpLabel->setColor(Color3B(100, 255, 100));
    this->addChild(_hpLabel);

    updateData(player);
    return true;
}

void AttributePanel::updateData(const PlayerData& player) {
    _atkLabel->setString("ATK: " + std::to_string(player.attack));
    _defLabel->setString("DEF: " + std::to_string(player.defense));
    _hpLabel->setString("HP: " + std::to_string(player.hp));
}