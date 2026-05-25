#include "Scenes/BattleScene.h"
#include "Scenes/ResultScene.h"
#include "Managers/DataManager.h"
#include "Constants.h"

USING_NS_CC;

Scene* BattleScene::createScene(int levelId) {
    auto scene = BattleScene::create();
    scene->setLevelId(levelId);
    return scene;
}

void BattleScene::setLevelId(int id) {
    _levelId = id;
    // Update title after levelId is set (init() runs before setLevelId)
    if (_titleLabel) {
        auto* level = DataManager::getInstance()->getLevel(_levelId);
        _titleLabel->setString(level ? "Battle: " + level->name : "Battle");
    }
}

bool BattleScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float centerX = origin.x + visibleSize.width / 2;
    float centerY = origin.y + visibleSize.height / 2;

    _elapsed = 0;

    // Dark dungeon background with subtle gradient
    auto bgTop = LayerColor::create(Color4B(50, 25, 25, 255));
    this->addChild(bgTop);
    auto bgBot = LayerColor::create(Color4B(25, 15, 15, 255));
    bgBot->setPosition(Vec2(origin.x, origin.y));
    bgBot->setContentSize(Size(visibleSize.width, visibleSize.height / 2));
    this->addChild(bgBot);

    // Floor pattern (horizontal lines)
    auto floorDraw = DrawNode::create();
    float floorY = origin.y + visibleSize.height * 0.3f;
    for (int i = 0; i < 8; i++) {
        float y = floorY - i * 15;
        float alpha = 0.15f - i * 0.015f;
        floorDraw->drawLine(Vec2(origin.x + 100, y), Vec2(origin.x + visibleSize.width - 100, y),
                            Color4F(0.5f, 0.4f, 0.3f, alpha > 0 ? alpha : 0.05f));
    }
    this->addChild(floorDraw);

    // Battle arena border
    auto arenaBorder = DrawNode::create();
    float arenaW = visibleSize.width * 0.7f;
    float arenaH = visibleSize.height * 0.4f;
    arenaBorder->drawRect(Vec2(centerX - arenaW/2, centerY - arenaH/2),
                          Vec2(centerX + arenaW/2, centerY + arenaH/2),
                          Color4F(0.6f, 0.3f, 0.3f, 0.3f));
    this->addChild(arenaBorder);

    // Player silhouette (left side)
    auto playerDraw = DrawNode::create();
    float px = centerX - arenaW/2 + 80;
    float py = centerY;
    // Body
    playerDraw->drawSolidCircle(Vec2(px, py + 20), 15, 0, 12, Color4F(0.39f, 0.78f, 0.39f, 0.9f));
    playerDraw->drawSolidRect(Vec2(px - 10, py - 25), Vec2(px + 10, py + 20), Color4F(0.39f, 0.78f, 0.39f, 0.9f));
    // Sword
    playerDraw->drawSolidRect(Vec2(px + 15, py - 5), Vec2(px + 55, py + 3), Color4F(0.7f, 0.7f, 0.8f, 0.9f));
    playerDraw->drawSolidRect(Vec2(px + 10, py - 2), Vec2(px + 15, py + 10), Color4F(0.6f, 0.5f, 0.2f, 0.9f));
    this->addChild(playerDraw);

    // Enemy silhouette (right side, red tinted)
    auto enemyDraw = DrawNode::create();
    float ex = centerX + arenaW/2 - 80;
    float ey = centerY;
    enemyDraw->drawSolidCircle(Vec2(ex, ey + 25), 20, 0, 12, Color4F(0.71f, 0.2f, 0.2f, 0.9f));
    enemyDraw->drawSolidRect(Vec2(ex - 15, ey - 30), Vec2(ex + 15, ey + 25), Color4F(0.71f, 0.2f, 0.2f, 0.9f));
    // Enemy horns
    Vec2 hornL[] = {Vec2(ex - 15, ey + 30), Vec2(ex - 22, ey + 50), Vec2(ex - 5, ey + 30)};
    enemyDraw->drawSolidPoly(hornL, 3, Color4F(0.78f, 0.39f, 0.2f, 0.9f));
    Vec2 hornR[] = {Vec2(ex + 5, ey + 30), Vec2(ex + 22, ey + 50), Vec2(ex + 15, ey + 30)};
    enemyDraw->drawSolidPoly(hornR, 3, Color4F(0.78f, 0.39f, 0.2f, 0.9f));
    // Eyes
    enemyDraw->drawSolidCircle(Vec2(ex - 6, ey + 30), 3, 0, 6, Color4F(1.0f, 0.3f, 0.3f, 1));
    enemyDraw->drawSolidCircle(Vec2(ex + 6, ey + 30), 3, 0, 6, Color4F(1.0f, 0.3f, 0.3f, 1));
    this->addChild(enemyDraw);

    // Title banner at top
    auto bannerBg = DrawNode::create();
    bannerBg->drawSolidRect(Vec2(centerX - 220, origin.y + visibleSize.height - 100),
                            Vec2(centerX + 220, origin.y + visibleSize.height - 45),
                            Color4F(0.4f, 0.2f, 0.2f, 0.7f));
    this->addChild(bannerBg);

    auto title = Label::createWithTTF("Battle", "fonts/arial.ttf", 36);
    title->setPosition(Vec2(centerX, origin.y + visibleSize.height - 72));
    title->setColor(Color3B(255, 120, 120));
    this->addChild(title);
    _titleLabel = title;

    // Timer with decorative frame
    auto timerFrame = DrawNode::create();
    timerFrame->drawSolidCircle(Vec2(centerX, centerY), 40, 0, 24, Color4F(0.3f, 0.15f, 0.15f, 0.5f));
    timerFrame->drawCircle(Vec2(centerX, centerY), 40, 0, 24, false, Color4F(0.6f, 0.3f, 0.3f, 0.8f));
    this->addChild(timerFrame);

    _timerLabel = Label::createWithTTF("", "fonts/arial.ttf", 42);
    _timerLabel->setPosition(Vec2(centerX, centerY));
    _timerLabel->setColor(Color3B(255, 220, 180));
    this->addChild(_timerLabel);

    // Progress bar with styled frame
    float barW = 500;
    float barH = 24;
    float barY = centerY - 80;

    auto barFrame = DrawNode::create();
    barFrame->drawSolidRect(Vec2(centerX - barW/2 - 4, barY - barH/2 - 4),
                            Vec2(centerX + barW/2 + 4, barY + barH/2 + 4),
                            Color4F(0.3f, 0.3f, 0.3f, 1));
    this->addChild(barFrame);

    auto barBg = Sprite::create();
    barBg->setTextureRect(Rect(0, 0, barW, barH));
    barBg->setColor(Color3B(60, 40, 40));
    barBg->setPosition(Vec2(centerX, barY));
    this->addChild(barBg);

    auto barFill = Sprite::create();
    barFill->setTextureRect(Rect(0, 0, barW, barH));
    barFill->setColor(Color3B(200, 120, 60));
    _progressBar = ProgressTimer::create(barFill);
    _progressBar->setType(ProgressTimer::Type::BAR);
    _progressBar->setMidpoint(Vec2(0, 0));
    _progressBar->setBarChangeRate(Vec2(1, 0));
    _progressBar->setPosition(Vec2(centerX, barY));
    _progressBar->setPercentage(0);
    this->addChild(_progressBar);

    // Tip with pulsing animation
    auto tip = Label::createWithTTF("Tap to speed up!", "fonts/arial.ttf", 18);
    tip->setPosition(Vec2(centerX, origin.y + visibleSize.height * 0.12f));
    tip->setColor(Color3B(180, 150, 120));
    tip->runAction(RepeatForever::create(Sequence::create(
        FadeTo::create(1.5f, 100),
        FadeTo::create(1.5f, 255),
        nullptr
    )));
    this->addChild(tip);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [this, enemyDraw](Touch* touch, Event* event) {
        _elapsed += 1.0f;
        // Enemy flash on hit
        enemyDraw->runAction(Sequence::create(
            ScaleBy::create(0.1f, 1.1f),
            ScaleTo::create(0.1f, 1.0f),
            nullptr
        ));
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    this->schedule(CC_SCHEDULE_SELECTOR(BattleScene::updateBattle), 0.1f);

    return true;
}

void BattleScene::updateBattle(float dt) {
    _elapsed += dt;

    float percentage = (_elapsed / BATTLE_DURATION) * 100.0f;
    _progressBar->setPercentage(std::min(percentage, 100.0f));

    float remaining = BATTLE_DURATION - _elapsed;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float centerX = origin.x + visibleSize.width / 2;
    float centerY = origin.y + visibleSize.height / 2;

    if (remaining <= 0) {
        _timerLabel->setString("Victory!");
        _timerLabel->setColor(colorGold());
        this->unschedule(CC_SCHEDULE_SELECTOR(BattleScene::updateBattle));
        onBattleComplete();
    } else {
        _timerLabel->setString(std::to_string((int)remaining + 1));
        // Timer color urgency: red when low time
        if (remaining < 2.0f) {
            _timerLabel->setColor(Color3B(255, 80, 80));
        } else if (remaining < 3.0f) {
            _timerLabel->setColor(Color3B(255, 180, 100));
        } else {
            _timerLabel->setColor(Color3B(255, 220, 180));
        }
    }
}

void BattleScene::onBattleComplete() {
    int stars = (rand() % 3) + 1;
    auto resultScene = ResultScene::createScene(_levelId, stars);
    auto transition = TransitionFade::create(0.5f, resultScene);
    Director::getInstance()->replaceScene(transition);
}