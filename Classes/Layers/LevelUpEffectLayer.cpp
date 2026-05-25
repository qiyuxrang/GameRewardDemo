#include "Layers/LevelUpEffectLayer.h"
#include "Managers/DataManager.h"
#include "Constants.h"

USING_NS_CC;

LevelUpEffectLayer* LevelUpEffectLayer::create(int newLevel) {
    auto* layer = new LevelUpEffectLayer();
    if (layer && layer->init(newLevel)) {
        layer->autorelease();
        return layer;
    }
    delete layer;
    return nullptr;
}

bool LevelUpEffectLayer::init(int newLevel) {
    if (!Layer::init()) return false;

    _newLevel = newLevel;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float centerX = origin.x + visibleSize.width / 2;
    float centerY = origin.y + visibleSize.height / 2;

    // Full-screen flash overlay
    auto flash = LayerColor::create(Color4B(255, 255, 255, 0));
    flash->setName("flash");
    this->addChild(flash, 10);

    // Background dark overlay for contrast
    auto darkOverlay = LayerColor::create(Color4B(0, 0, 0, 160));
    darkOverlay->setName("darkOverlay");
    darkOverlay->setOpacity(0);
    this->addChild(darkOverlay, 5);

    // Decorative star burst around text
    auto burstDraw = DrawNode::create();
    burstDraw->setName("burst");
    for (int i = 0; i < 16; i++) {
        float angle = i * 22.5f * (float)M_PI / 180.0f;
        float len = 120.0f;
        float inner = 60.0f;
        burstDraw->drawLine(Vec2(centerX + inner * cos(angle), centerY + 50 + inner * sin(angle)),
                            Vec2(centerX + len * cos(angle), centerY + 50 + len * sin(angle)),
                            Color4F(1.0f, 0.84f, 0.0f, 0.3f));
    }
    this->addChild(burstDraw, 12);

    // LEVEL UP text with glow
    auto levelUpText = Label::createWithTTF("LEVEL UP!", "fonts/arial.ttf", 64);
    levelUpText->setPosition(Vec2(centerX, centerY + 50));
    levelUpText->setColor(colorGold());
    levelUpText->enableGlow(Color4B(255, 215, 0, 150));
    levelUpText->setScale(0);
    levelUpText->setName("levelUpText");
    this->addChild(levelUpText, 20);

    // New level number
    auto levelNum = Label::createWithTTF("Lv." + std::to_string(_newLevel), "fonts/arial.ttf", 48);
    levelNum->setPosition(Vec2(centerX, centerY - 20));
    levelNum->setColor(Color3B(100, 255, 100));
    levelNum->enableGlow(Color4B(100, 255, 100, 100));
    levelNum->setScale(0);
    levelNum->setName("levelNum");
    this->addChild(levelNum, 20);

    // Attribute change panel
    auto& player = DataManager::getInstance()->getPlayer();
    float panelY = centerY - 80;
    auto atkLabel = Label::createWithTTF("ATK +2", "fonts/arial.ttf", 22);
    atkLabel->setPosition(Vec2(centerX - 80, panelY));
    atkLabel->setColor(Color3B(255, 100, 100));
    atkLabel->setOpacity(0);
    atkLabel->setName("atkChange");
    this->addChild(atkLabel, 20);

    auto defLabel = Label::createWithTTF("DEF +1", "fonts/arial.ttf", 22);
    defLabel->setPosition(Vec2(centerX, panelY));
    defLabel->setColor(Color3B(100, 100, 255));
    defLabel->setOpacity(0);
    defLabel->setName("defChange");
    this->addChild(defLabel, 20);

    auto hpLabel = Label::createWithTTF("HP +20", "fonts/arial.ttf", 22);
    hpLabel->setPosition(Vec2(centerX + 80, panelY));
    hpLabel->setColor(Color3B(100, 255, 100));
    hpLabel->setOpacity(0);
    hpLabel->setName("hpChange");
    this->addChild(hpLabel, 20);

    // Small floating gold particles
    for (int i = 0; i < 12; i++) {
        auto particle = DrawNode::create();
        particle->drawSolidCircle(Vec2(0, 0), 4, 0, 6, Color4F(1.0f, 0.84f, 0.0f, 0.8f));
        particle->setPosition(Vec2(centerX + (rand() % 200 - 100),
                                   centerY + (rand() % 100 - 50)));
        particle->setName("particle" + std::to_string(i));
        this->addChild(particle, 15);
    }

    // Animation sequence: flash -> dark overlay -> text -> attributes -> particles -> fade out
    this->runAction(Sequence::create(
        CallFunc::create([this]() { playFlashEffect(); }),
        DelayTime::create(0.3f),
        CallFunc::create([this]() { playDarkOverlay(); }),
        DelayTime::create(0.3f),
        CallFunc::create([this]() { playTextEffect(); }),
        DelayTime::create(0.4f),
        CallFunc::create([this]() { playAttributeEffect(); }),
        CallFunc::create([this]() { playParticleEffect(); }),
        DelayTime::create(2.0f),
        CallFunc::create([this]() { playComplete(); }),
        nullptr
    ));

    return true;
}

void LevelUpEffectLayer::setOnCompleteCallback(const std::function<void()>& callback) {
    _onComplete = callback;
}

void LevelUpEffectLayer::playFlashEffect() {
    auto flash = this->getChildByName("flash");
    if (flash) {
        flash->runAction(Sequence::create(
            FadeIn::create(0.08f),
            FadeOut::create(0.35f),
            nullptr
        ));
    }
}

void LevelUpEffectLayer::playDarkOverlay() {
    auto darkOverlay = this->getChildByName("darkOverlay");
    if (darkOverlay) {
        darkOverlay->runAction(FadeTo::create(0.3f, 160));
    }
}

void LevelUpEffectLayer::playTextEffect() {
    auto levelUpText = this->getChildByName("levelUpText");
    auto levelNum = this->getChildByName("levelNum");

    if (levelUpText) {
        levelUpText->runAction(Sequence::create(
            EaseBackOut::create(ScaleBy::create(0.5f, 1.5f)),
            ScaleTo::create(0.2f, 1.0f),
            nullptr
        ));
    }

    if (levelNum) {
        levelNum->runAction(Sequence::create(
            DelayTime::create(0.3f),
            EaseBackOut::create(ScaleBy::create(0.4f, 1.3f)),
            ScaleTo::create(0.2f, 1.0f),
            nullptr
        ));
    }

    // Burst rays rotate slightly
    auto burst = this->getChildByName("burst");
    if (burst) {
        burst->runAction(Repeat::create(RotateBy::create(3.0f, 45.0f), 1));
    }
}

void LevelUpEffectLayer::playAttributeEffect() {
    auto atkChange = this->getChildByName("atkChange");
    auto defChange = this->getChildByName("defChange");
    auto hpChange = this->getChildByName("hpChange");

    if (atkChange) {
        atkChange->runAction(Sequence::create(
            FadeIn::create(0.3f),
            MoveBy::create(0.3f, Vec2(0, 15)),
            nullptr
        ));
    }
    if (defChange) {
        defChange->runAction(Sequence::create(
            DelayTime::create(0.1f),
            FadeIn::create(0.3f),
            MoveBy::create(0.3f, Vec2(0, 15)),
            nullptr
        ));
    }
    if (hpChange) {
        hpChange->runAction(Sequence::create(
            DelayTime::create(0.2f),
            FadeIn::create(0.3f),
            MoveBy::create(0.3f, Vec2(0, 15)),
            nullptr
        ));
    }
}

void LevelUpEffectLayer::playParticleEffect() {
    for (int i = 0; i < 12; i++) {
        auto particle = this->getChildByName("particle" + std::to_string(i));
        if (particle) {
            particle->runAction(Sequence::create(
                Spawn::create(
                    MoveBy::create(1.5f, Vec2((rand() % 60 - 30), rand() % 80 + 30)),
                    FadeOut::create(1.5f),
                    nullptr
                ),
                nullptr
            ));
        }
    }
}

void LevelUpEffectLayer::playComplete() {
    auto levelUpText = this->getChildByName("levelUpText");
    auto levelNum = this->getChildByName("levelNum");
    auto atkChange = this->getChildByName("atkChange");
    auto defChange = this->getChildByName("defChange");
    auto hpChange = this->getChildByName("hpChange");
    auto darkOverlay = this->getChildByName("darkOverlay");

    if (levelUpText) levelUpText->runAction(FadeOut::create(0.3f));
    if (levelNum) levelNum->runAction(FadeOut::create(0.3f));
    if (atkChange) atkChange->runAction(FadeOut::create(0.3f));
    if (defChange) defChange->runAction(FadeOut::create(0.3f));
    if (hpChange) hpChange->runAction(FadeOut::create(0.3f));
    if (darkOverlay) darkOverlay->runAction(FadeOut::create(0.3f));

    this->runAction(Sequence::create(
        DelayTime::create(0.5f),
        CallFunc::create([this]() {
            if (_onComplete) _onComplete();
            this->removeFromParent();
        }),
        nullptr
    ));
}