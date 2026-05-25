#include "Scenes/ResultScene.h"
#include "Managers/DataManager.h"
#include "UI/RewardItemNode.h"
#include "UI/IconFactory.h"
#include "Layers/RewardFlyLayer.h"
#include "Layers/LevelUpEffectLayer.h"
#include "Scenes/LevelSelectScene.h"
#include "Constants.h"

USING_NS_CC;

Scene* ResultScene::createScene(int levelId, int stars) {
    auto scene = ResultScene::create();
    scene->setLevelResult(levelId, stars);
    return scene;
}

void ResultScene::setLevelResult(int levelId, int stars) {
    _levelId = levelId;
    _stars = stars;
    setupResultContent();
}

bool ResultScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Victory gradient background
    auto bgTop = LayerColor::create(Color4B(20, 40, 20, 255));
    this->addChild(bgTop);
    auto bgBot = LayerColor::create(Color4B(15, 30, 15, 255));
    bgBot->setContentSize(Size(visibleSize.width, visibleSize.height / 2));
    bgBot->setPosition(Vec2(origin.x, origin.y));
    this->addChild(bgBot);

    return true;
}

DrawNode* ResultScene::createStarShape(const Color4F& color) {
    auto starDraw = DrawNode::create();
    float outerR = 18.0f;
    float innerR = 7.0f;
    Vec2 verts[10];
    for (int p = 0; p < 5; p++) {
        float oa = (p * 72.0f - 90.0f) * (float)M_PI / 180.0f;
        float ia = ((p * 72.0f + 36.0f) - 90.0f) * (float)M_PI / 180.0f;
        verts[p * 2] = Vec2(outerR * cos(oa), outerR * sin(oa));
        verts[p * 2 + 1] = Vec2(innerR * cos(ia), innerR * sin(ia));
    }
    starDraw->drawSolidPoly(verts, 10, color);
    return starDraw;
}

void ResultScene::setupResultContent() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float centerX = origin.x + visibleSize.width / 2;

    // Decorative rays emanating from center
    auto raysDraw = DrawNode::create();
    float rayY = origin.y + visibleSize.height - 140;
    for (int i = 0; i < 12; i++) {
        float angle = i * 30.0f * (float)M_PI / 180.0f;
        float len = 180.0f;
        raysDraw->drawLine(Vec2(centerX, rayY),
                           Vec2(centerX + len * cos(angle), rayY + len * sin(angle) * 0.4f),
                           Color4F(1.0f, 0.84f, 0.0f, 0.15f));
    }
    this->addChild(raysDraw);

    // Victory title with glow effect
    auto title = Label::createWithTTF("Victory!", "fonts/arial.ttf", 42);
    title->setPosition(Vec2(centerX, origin.y + visibleSize.height - 170));
    title->setColor(colorCleared());
    title->enableGlow(Color4B(100, 255, 100, 80));
    this->addChild(title);

    // Phase 1: Play star fly-in animation, then show rewards
    playStarFlyAnimation();
}

void ResultScene::playStarFlyAnimation() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float centerX = origin.x + visibleSize.width / 2;
    float starY = origin.y + visibleSize.height - 110;

    // Stars launch from panel center area
    float panelCenterY = origin.y + visibleSize.height / 2;
    int earnedStars = _stars;
    auto flyDone = std::make_shared<int>(0);

    for (int i = 0; i < 3; i++) {
        float sx = centerX + (i - 1) * 40;
        bool earned = (i < earnedStars);

        if (earned) {
            // Gold star flies from panel center to top
            auto flyStar = createStarShape(Color4F(1.0f, 0.84f, 0.0f, 1));
            flyStar->setScale(0.3f);
            flyStar->setPosition(Vec2(centerX, panelCenterY));
            this->addChild(flyStar, 15);

            Vec2 targetPos(sx, starY);
            float delay = i * 0.2f;

            // Bezier arc: lift up then curve into star slot
            ccBezierConfig bezier;
            bezier.controlPoint_1 = Vec2(centerX + (sx - centerX) * 0.3f, panelCenterY + 180);
            bezier.controlPoint_2 = Vec2(sx, starY + 120);
            bezier.endPosition = targetPos;

            flyStar->runAction(Sequence::create(
                DelayTime::create(delay),
                EaseBackOut::create(ScaleTo::create(0.15f, 1.3f)),
                ScaleTo::create(0.08f, 1.0f),
                Spawn::create(
                    BezierTo::create(0.5f, bezier),
                    Sequence::create(
                        ScaleTo::create(0.15f, 1.2f),
                        ScaleTo::create(0.35f, 1.0f),
                        nullptr
                    ),
                    nullptr
                ),
                EaseBounceOut::create(ScaleBy::create(0.3f, 0.25f)),
                ScaleTo::create(0.1f, 1.0f),
                // Replace flying star with settled star
                CallFunc::create([this, targetPos]() {
                    auto settledStar = createStarShape(Color4F(1.0f, 0.84f, 0.0f, 1));
                    settledStar->setPosition(targetPos);
                    this->addChild(settledStar, 5);
                }),
                RemoveSelf::create(),
                CallFunc::create([this, flyDone, earnedStars]() {
                    (*flyDone)++;
                    if (*flyDone >= earnedStars) {
                        showGrayStars();
                        showRewardPanel();
                    }
                }),
                nullptr
            ));
        } else {
            // Gray empty star - will be shown after earned stars land
            auto grayStar = createStarShape(Color4F(0.3f, 0.3f, 0.3f, 0.6f));
            grayStar->setPosition(Vec2(sx, starY));
            grayStar->setOpacity(0);
            grayStar->setName("grayStar" + std::to_string(i));
            this->addChild(grayStar, 5);
        }
    }

    // No earned stars - show everything immediately
    if (earnedStars == 0) {
        showGrayStars();
        showRewardPanel();
    }
}

void ResultScene::showGrayStars() {
    for (int i = 0; i < 3; i++) {
        auto grayStar = this->getChildByName("grayStar" + std::to_string(i));
        if (grayStar) {
            grayStar->runAction(FadeIn::create(0.4f));
        }
    }
}

void ResultScene::showRewardPanel() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float centerX = origin.x + visibleSize.width / 2;

    auto* level = DataManager::getInstance()->getLevel(_levelId);
    float panelH = level ? (float)level->rewards.size() * 60 + 40 : 120;
    float panelW = 500;
    float panelTop = origin.y + visibleSize.height / 2 + 60;
    float panelBot = panelTop - panelH;

    // Reward panel slides in from right
    auto panelFrame = DrawNode::create();
    panelFrame->drawSolidRect(Vec2(centerX - panelW/2, panelBot),
                              Vec2(centerX + panelW/2, panelTop),
                              Color4F(0.16f, 0.2f, 0.16f, 0.7f));
    panelFrame->drawRect(Vec2(centerX - panelW/2, panelBot),
                         Vec2(centerX + panelW/2, panelTop),
                         Color4F(0.39f, 0.78f, 0.39f, 0.4f));
    panelFrame->setPosition(Vec2(visibleSize.width, 0));
    panelFrame->runAction(EaseBackOut::create(MoveBy::create(0.4f, Vec2(-visibleSize.width, 0))));
    this->addChild(panelFrame);

    // "Rewards" header
    auto rewardHeader = Label::createWithTTF("Rewards", "fonts/arial.ttf", 22);
    rewardHeader->setPosition(Vec2(centerX, panelTop - 15));
    rewardHeader->setColor(Color3B(180, 220, 180));
    rewardHeader->setOpacity(0);
    rewardHeader->runAction(Sequence::create(
        DelayTime::create(0.3f),
        FadeIn::create(0.3f),
        nullptr
    ));
    this->addChild(rewardHeader);

    buildRewardList();

    // Collect button fades in
    float btnY = origin.y + 100;
    auto btnBg = DrawNode::create();
    btnBg->drawSolidRect(Vec2(centerX - 100, btnY - 25), Vec2(centerX + 100, btnY + 25),
                         Color4F(1.0f, 0.84f, 0.0f, 0.3f));
    btnBg->drawRect(Vec2(centerX - 100, btnY - 25), Vec2(centerX + 100, btnY + 25),
                    Color4F(1.0f, 0.84f, 0.0f, 0.8f));
    btnBg->setOpacity(0);
    btnBg->runAction(Sequence::create(
        DelayTime::create(0.5f),
        FadeIn::create(0.3f),
        nullptr
    ));
    this->addChild(btnBg);

    auto collectBtn = MenuItemLabel::create(
        Label::createWithTTF("Collect", "fonts/arial.ttf", 30),
        [this](Ref* sender) {
            onCollectClicked();
        }
    );
    collectBtn->setColor(colorGold());
    collectBtn->setOpacity(0);
    collectBtn->runAction(Sequence::create(
        DelayTime::create(0.5f),
        FadeIn::create(0.3f),
        nullptr
    ));

    auto menu = Menu::create(collectBtn, nullptr);
    menu->setPosition(Vec2(centerX, btnY));
    this->addChild(menu, 1, 100);
}

void ResultScene::buildRewardList() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float centerX = origin.x + visibleSize.width / 2;

    auto* level = DataManager::getInstance()->getLevel(_levelId);
    if (!level) return;

    float startY = origin.y + visibleSize.height / 2 + 20;
    float spacing = 60;

    for (size_t i = 0; i < level->rewards.size(); i++) {
        auto& reward = level->rewards[i];
        float yPos = startY - i * spacing;

        // Reward row background
        auto rowBg = DrawNode::create();
        rowBg->drawSolidRect(Vec2(centerX - 230, yPos - 22), Vec2(centerX + 230, yPos + 22),
                              Color4F(0.2f, 0.24f, 0.2f, 0.4f));
        this->addChild(rowBg);

        auto itemNode = RewardItemNode::create(reward.first, reward.second);
        itemNode->setPosition(Vec2(centerX, yPos));
        itemNode->setOpacity(0);
        // Stagger fade-in for each reward row
        itemNode->runAction(Sequence::create(
            DelayTime::create(0.35f + i * 0.1f),
            FadeIn::create(0.25f),
            nullptr
        ));
        this->addChild(itemNode, 5);
        _rewardNodes.pushBack(itemNode);
    }
}

void ResultScene::onCollectClicked() {
    DataManager::getInstance()->completeLevel(_levelId, _stars);

    auto* level = DataManager::getInstance()->getLevel(_levelId);
    if (!level) return;

    // Disable further clicks
    this->removeChildByTag(100);

    // Capture reward node positions before removing them
    std::vector<Vec2> startPositions;
    for (auto& node : _rewardNodes) {
        startPositions.push_back(node->getPosition());
    }
    for (auto& node : _rewardNodes) {
        node->removeFromParent();
    }
    _rewardNodes.clear();

    // Reward items fly from panel to inventory bar
    auto rewardFlyLayer = RewardFlyLayer::create(level->rewards, startPositions);
    this->addChild(rewardFlyLayer, 10);

    rewardFlyLayer->setOnCompleteCallback([this]() {
        auto& player = DataManager::getInstance()->getPlayer();

        if (player.canLevelUp()) {
            player.levelUp();
            auto levelUpEffect = LevelUpEffectLayer::create(player.level);
            this->addChild(levelUpEffect, 20);

            levelUpEffect->setOnCompleteCallback([]() {
                auto levelSelectScene = LevelSelectScene::createScene();
                auto transition = TransitionFade::create(0.5f, levelSelectScene);
                Director::getInstance()->replaceScene(transition);
            });
        } else {
            auto levelSelectScene = LevelSelectScene::createScene();
            auto transition = TransitionFade::create(0.5f, levelSelectScene);
            Director::getInstance()->replaceScene(transition);
        }
    });
}