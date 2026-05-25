#include "Layers/RewardFlyLayer.h"
#include "UI/IconFactory.h"
#include "Constants.h"

USING_NS_CC;

RewardFlyLayer* RewardFlyLayer::create(const std::vector<std::pair<ItemData, int>>& rewards,
                                        const std::vector<cocos2d::Vec2>& startPositions) {
    auto* layer = new RewardFlyLayer();
    if (layer && layer->init(rewards, startPositions)) {
        layer->autorelease();
        return layer;
    }
    delete layer;
    return nullptr;
}

bool RewardFlyLayer::init(const std::vector<std::pair<ItemData, int>>& rewards,
                          const std::vector<cocos2d::Vec2>& startPositions) {
    if (!Layer::init()) return false;

    _rewards = rewards;
    _startPositions = startPositions;
    _flyCount = (int)rewards.size();
    _flyDone = 0;

    // Subtle dark overlay to focus attention on flying items
    auto mask = LayerColor::create(Color4B(0, 0, 0, 60));
    this->addChild(mask, -10);

    // Bottom target strip - visual destination for fly items
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float centerX = origin.x + visibleSize.width / 2;

    auto targetStrip = Sprite::create();
    targetStrip->setTextureRect(Rect(0, 0, visibleSize.width, 90));
    targetStrip->setColor(Color3B(30, 30, 50));
    targetStrip->setOpacity(160);
    targetStrip->setPosition(Vec2(centerX, origin.y + 45));
    targetStrip->setName("targetStrip");
    targetStrip->setOpacity(0);
    this->addChild(targetStrip, -5);

    // Slot markers in target strip
    float slotWidth = 80;
    float totalSlotsWidth = INVENTORY_MAX_SLOTS * slotWidth;
    float slotsStartX = origin.x + (visibleSize.width - totalSlotsWidth) / 2 + slotWidth / 2;

    for (int i = 0; i < std::min(_flyCount, INVENTORY_MAX_SLOTS); i++) {
        float slotX = slotsStartX + i * slotWidth;
        float slotY = origin.y + 45;
        auto slotBg = Sprite::create();
        slotBg->setTextureRect(Rect(0, 0, 60, 60));
        slotBg->setColor(Color3B(60, 60, 80));
        slotBg->setOpacity(0);
        slotBg->setPosition(Vec2(slotX, slotY));
        slotBg->setName("targetSlot" + std::to_string(i));
        this->addChild(slotBg, -3);
    }

    // Fade in target strip after brief delay
    targetStrip->runAction(Sequence::create(
        DelayTime::create(0.15f),
        FadeTo::create(0.3f, 160),
        nullptr
    ));

    for (int i = 0; i < std::min(_flyCount, INVENTORY_MAX_SLOTS); i++) {
        auto slot = this->getChildByName("targetSlot" + std::to_string(i));
        if (slot) {
            slot->runAction(Sequence::create(
                DelayTime::create(0.15f),
                FadeTo::create(0.3f, 200),
                nullptr
            ));
        }
    }

    this->runAction(Sequence::create(
        DelayTime::create(0.2f),
        CallFunc::create([this]() { startFlyAnimation(); }),
        nullptr
    ));

    return true;
}

void RewardFlyLayer::setOnCompleteCallback(const std::function<void()>& callback) {
    _onComplete = callback;
}

Vec2 RewardFlyLayer::getInventoryTargetPos(int slotIndex) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float slotWidth = 80;
    float totalSlotsWidth = INVENTORY_MAX_SLOTS * slotWidth;
    float startX = origin.x + (visibleSize.width - totalSlotsWidth) / 2 + slotWidth / 2;
    float yPos = origin.y + 45;
    return Vec2(startX + slotIndex * slotWidth, yPos);
}

void RewardFlyLayer::startFlyAnimation() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    for (int i = 0; i < _flyCount; i++) {
        auto& reward = _rewards[i];
        auto& itemData = reward.first;

        // Use IconFactory for recognizable shape icons
        auto flyIcon = IconFactory::createItemIcon(itemData.type, 50.0f);
        flyIcon->setScale(0.1f);  // Start small for pop-out effect

        // Start from the reward row position
        Vec2 startPos = (i < (int)_startPositions.size()) ? _startPositions[i] :
            Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 100);
        flyIcon->setPosition(startPos);
        this->addChild(flyIcon, 5);

        Vec2 targetPos = getInventoryTargetPos(i);
        float delay = i * FLY_INTERVAL;

        // Bezier arc: lift off upward, then swoop down into slot
        ccBezierConfig bezier;
        bezier.controlPoint_1 = Vec2(startPos.x, startPos.y + 120);
        bezier.controlPoint_2 = Vec2(targetPos.x, targetPos.y + 180);
        bezier.endPosition = targetPos;

        // Departure spark at start position
        auto spark = DrawNode::create();
        spark->drawSolidCircle(Vec2(startPos.x, startPos.y), 30, 0, 16, Color4F(1.0f, 0.84f, 0.0f, 0.4f));
        spark->setName("spark" + std::to_string(i));
        spark->setOpacity(0);
        this->addChild(spark, 3);

        // Full fly animation sequence
        auto flyAction = Sequence::create(
            DelayTime::create(delay),
            // Pop-out at departure point
            EaseBackOut::create(ScaleTo::create(0.2f, 1.2f)),
            ScaleTo::create(0.1f, 1.0f),
            // Spark flash at departure
            CallFunc::create([this, i]() {
                auto s = this->getChildByName("spark" + std::to_string(i));
                if (s) s->runAction(Sequence::create(
                    FadeIn::create(0.05f),
                    FadeOut::create(0.3f),
                    RemoveSelf::create(),
                    nullptr
                ));
            }),
            // Arc flight + shrink during flight
            Spawn::create(
                BezierTo::create(FLY_DURATION, bezier),
                Sequence::create(
                    ScaleTo::create(FLY_DURATION * 0.3f, 0.9f),
                    ScaleTo::create(FLY_DURATION * 0.7f, 0.6f),
                    nullptr
                ),
                nullptr
            ),
            // Arrival bounce
            ScaleTo::create(0.1f, 0.8f),
            ScaleTo::create(0.1f, 0.6f),
            // Slot highlight on arrival
            CallFunc::create([this, i]() {
                auto slot = this->getChildByName("targetSlot" + std::to_string(i));
                if (slot) {
                    slot->runAction(Sequence::create(
                        ScaleTo::create(0.1f, 1.15f),
                        ScaleTo::create(0.1f, 1.0f),
                        nullptr
                    ));
                }
            }),
            // Fade out fly icon
            FadeOut::create(0.2f),
            CallFunc::create([this, flyIcon]() {
                flyIcon->removeFromParent();
                _flyDone++;
                if (_flyDone >= _flyCount && _onComplete) {
                    _onComplete();
                }
            }),
            nullptr
        );

        flyIcon->runAction(flyAction);

        // Gold coin particle trail - starts from same position, follows straight line to target
        if (itemData.type == ItemType::GOLD) {
            for (int p = 0; p < 4; p++) {
                auto particle = IconFactory::createItemIcon(ItemType::GOLD, 10.0f);
                particle->setOpacity(0);
                particle->setPosition(startPos);
                this->addChild(particle, 2);

                float pDelay = delay + 0.25f + p * 0.06f;
                float pDur = FLY_DURATION * 0.7f;
                auto pAction = Sequence::create(
                    DelayTime::create(pDelay),
                    Spawn::create(
                        MoveTo::create(pDur,
                                      Vec2(targetPos.x + (rand() % 16 - 8),
                                           targetPos.y + (rand() % 16 - 8))),
                        Sequence::create(
                            FadeIn::create(0.08f),
                            FadeOut::create(pDur),
                            nullptr
                        ),
                        nullptr
                    ),
                    RemoveSelf::create(),
                    nullptr
                );
                particle->runAction(pAction);
            }
        }
    }
}