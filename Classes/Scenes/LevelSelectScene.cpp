#include "Scenes/LevelSelectScene.h"
#include "Managers/DataManager.h"
#include "Scenes/BattleScene.h"
#include "Constants.h"

USING_NS_CC;

Scene* LevelSelectScene::createScene() {
    return LevelSelectScene::create();
}

bool LevelSelectScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float centerX = origin.x + visibleSize.width / 2;

    // Background gradient effect
    auto bg = LayerColor::create(Color4B(30, 30, 50, 255));
    this->addChild(bg);

    // Title with decorative underline
    auto title = Label::createWithTTF("Level Select", "fonts/arial.ttf", 40);
    title->setPosition(Vec2(centerX, origin.y + visibleSize.height - 60));
    title->setColor(colorGold());
    this->addChild(title);

    auto titleLine = DrawNode::create();
    titleLine->drawSolidRect(Vec2(centerX - 180, origin.y + visibleSize.height - 80),
                              Vec2(centerX + 180, origin.y + visibleSize.height - 82),
                              Color4F(1.0f, 0.84f, 0.0f, 0.6f));
    this->addChild(titleLine);

    // Player info panel with icon and exp bar
    auto panelBg = DrawNode::create();
    panelBg->drawSolidRect(Vec2(origin.x + 20, origin.y + 20),
                           Vec2(origin.x + visibleSize.width - 20, origin.y + 110),
                           Color4F(0.16f, 0.16f, 0.24f, 0.78f));
    this->addChild(panelBg);

    // Player level icon (shield shape)
    auto playerIcon = DrawNode::create();
    float iconY = origin.y + 65;
    float iconX = origin.x + 60;
    playerIcon->drawSolidCircle(Vec2(iconX, iconY), 20, 0, 16, Color4F(0.39f, 0.78f, 0.39f, 1));
    playerIcon->drawSolidRect(Vec2(iconX - 5, iconY - 10), Vec2(iconX + 5, iconY + 12),
                              Color4F(0.78f, 0.78f, 0.78f, 1));
    playerIcon->drawSolidRect(Vec2(iconX - 12, iconY + 2), Vec2(iconX + 12, iconY + 7),
                              Color4F(0.78f, 0.78f, 0.78f, 1));
    this->addChild(playerIcon);

    _levelLabel = Label::createWithTTF("", "fonts/arial.ttf", 24);
    _levelLabel->setPosition(Vec2(iconX + 40, iconY + 10));
    _levelLabel->setColor(colorGold());
    this->addChild(_levelLabel);

    // Exp bar background
    float barX = origin.x + 120;
    float barW = visibleSize.width - 200;
    auto barBg = Sprite::create();
    barBg->setTextureRect(Rect(0, 0, barW, 16));
    barBg->setColor(Color3B(60, 60, 80));
    barBg->setPosition(Vec2(barX + barW / 2, iconY - 15));
    this->addChild(barBg);

    // Exp bar fill
    auto barFill = Sprite::create();
    barFill->setTextureRect(Rect(0, 0, barW, 16));
    barFill->setColor(Color3B(100, 255, 100));
    _expBar = ProgressTimer::create(barFill);
    _expBar->setType(ProgressTimer::Type::BAR);
    _expBar->setMidpoint(Vec2(0, 0));
    _expBar->setBarChangeRate(Vec2(1, 0));
    _expBar->setPosition(Vec2(barX + barW / 2, iconY - 15));
    _expBar->setPercentage(0);
    this->addChild(_expBar);

    // Exp label on bar
    _expLabel = Label::createWithTTF("", "fonts/arial.ttf", 14);
    _expLabel->setPosition(Vec2(barX + barW / 2, iconY - 15));
    _expLabel->setColor(Color3B(255, 255, 255));
    this->addChild(_expLabel);

    // Gold display
    auto goldIcon = DrawNode::create();
    float goldX = origin.x + visibleSize.width - 80;
    goldIcon->drawSolidCircle(Vec2(goldX, iconY), 14, 0, 16, Color4F(1.0f, 0.84f, 0.0f, 1));
    goldIcon->drawSolidCircle(Vec2(goldX + 3, iconY + 3), 4, 0, 8, Color4F(1.0f, 0.95f, 0.5f, 0.8f));
    this->addChild(goldIcon);

    _goldLabel = Label::createWithTTF("", "fonts/arial.ttf", 20);
    _goldLabel->setPosition(Vec2(goldX - 30, iconY));
    _goldLabel->setColor(colorGold());
    this->addChild(_goldLabel);

    buildLevelNodes();
    updatePlayerInfo();

    return true;
}

void LevelSelectScene::buildLevelNodes() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto& levels = DataManager::getInstance()->getLevels();
    float startX = origin.x + 160;
    float spacing = (visibleSize.width - 320) / (levels.size() - 1);
    float nodeY = origin.y + visibleSize.height / 2 + 40;

    for (size_t i = 0; i < levels.size(); i++) {
        auto& level = levels[i];
        float xPos = startX + i * spacing;

        // Level icon based on level theme
        auto levelIcon = DrawNode::create();
        float iconSize = 45;

        // Draw level-specific icon
        switch (level.id) {
            case 1: { // Trial - training dummy / sword
                levelIcon->drawSolidRect(Vec2(-4, -iconSize*0.3f), Vec2(4, iconSize*0.7f),
                                         Color4F(0.7f, 0.7f, 0.8f, 1));
                levelIcon->drawSolidRect(Vec2(-iconSize*0.3f, -iconSize*0.2f),
                                         Vec2(iconSize*0.3f, -iconSize*0.1f),
                                         Color4F(0.6f, 0.5f, 0.2f, 1));
                levelIcon->drawSolidRect(Vec2(-iconSize*0.15f, iconSize*0.7f),
                                         Vec2(iconSize*0.15f, iconSize*0.85f),
                                         Color4F(0.55f, 0.27f, 0.07f, 1));
                break;
            }
            case 2: { // Dark Forest - tree
                levelIcon->drawSolidCircle(Vec2(0, iconSize*0.2f), iconSize*0.5f, 0, 8,
                                           Color4F(0.2f, 0.5f, 0.2f, 1));
                levelIcon->drawSolidRect(Vec2(-3, -iconSize*0.4f), Vec2(3, iconSize*0.1f),
                                         Color4F(0.4f, 0.2f, 0.1f, 1));
                levelIcon->drawSolidCircle(Vec2(-iconSize*0.3f, iconSize*0.35f), iconSize*0.3f, 0, 8,
                                           Color4F(0.15f, 0.4f, 0.15f, 1));
                break;
            }
            case 3: { // Dragon Nest - flame
                levelIcon->drawSolidCircle(Vec2(0, -iconSize*0.1f), iconSize*0.35f, 0, 16,
                                           Color4F(1.0f, 0.3f, 0.0f, 1));
                Vec2 flameOuter[] = {Vec2(-iconSize*0.3f, -iconSize*0.2f),
                                     Vec2(0, iconSize*0.6f),
                                     Vec2(iconSize*0.3f, -iconSize*0.2f)};
                levelIcon->drawSolidPoly(flameOuter, 3, Color4F(1.0f, 0.5f, 0.1f, 1));
                Vec2 flameInner[] = {Vec2(-iconSize*0.15f, 0),
                                     Vec2(0, iconSize*0.4f),
                                     Vec2(iconSize*0.15f, 0)};
                levelIcon->drawSolidPoly(flameInner, 3, Color4F(1.0f, 0.8f, 0.2f, 1));
                break;
            }
            case 4: { // Abyss Gate - portal rings
                levelIcon->drawSolidCircle(Vec2(0, 0), iconSize*0.5f, 0, 24,
                                           Color4F(0.2f, 0.1f, 0.3f, 1));
                levelIcon->drawSolidCircle(Vec2(0, 0), iconSize*0.35f, 0, 24,
                                           Color4F(0.4f, 0.15f, 0.5f, 1));
                levelIcon->drawSolidCircle(Vec2(0, 0), iconSize*0.2f, 0, 24,
                                           Color4F(0.6f, 0.3f, 0.8f, 1));
                break;
            }
            case 5: { // Final Trial - crown
                float crownW = iconSize * 0.8f;
                float crownH = iconSize * 0.4f;
                float crownY = iconSize * 0.1f;
                // Crown base
                levelIcon->drawSolidRect(Vec2(-crownW/2, crownY), Vec2(crownW/2, crownY + crownH*0.4f),
                                         Color4F(1.0f, 0.84f, 0.0f, 1));
                // Crown points
                Vec2 cp1[] = {Vec2(-crownW/2, crownY + crownH*0.4f),
                              Vec2(-crownW*0.35f, crownY + crownH),
                              Vec2(-crownW*0.2f, crownY + crownH*0.4f)};
                levelIcon->drawSolidPoly(cp1, 3, Color4F(1.0f, 0.84f, 0.0f, 1));
                Vec2 cp2[] = {Vec2(-crownW*0.1f, crownY + crownH*0.4f),
                              Vec2(0, crownY + crownH*1.1f),
                              Vec2(crownW*0.1f, crownY + crownH*0.4f)};
                levelIcon->drawSolidPoly(cp2, 3, Color4F(1.0f, 0.84f, 0.0f, 1));
                Vec2 cp3[] = {Vec2(crownW*0.2f, crownY + crownH*0.4f),
                              Vec2(crownW*0.35f, crownY + crownH),
                              Vec2(crownW/2, crownY + crownH*0.4f)};
                levelIcon->drawSolidPoly(cp3, 3, Color4F(1.0f, 0.84f, 0.0f, 1));
                // Crown gems
                levelIcon->drawSolidCircle(Vec2(0, crownY + crownH*0.5f), 4, 0, 8,
                                           Color4F(0.8f, 0.2f, 0.2f, 1));
                break;
            }
        }

        // Node background circle with border
        Color4F bgColor, borderColor;
        switch (level.state) {
            case LevelState::CLEARED:
                bgColor = Color4F(0.3f, 0.5f, 0.3f, 0.8f);
                borderColor = Color4F(0.5f, 0.8f, 0.5f, 1);
                break;
            case LevelState::AVAILABLE:
                bgColor = Color4F(0.5f, 0.5f, 0.2f, 0.8f);
                borderColor = Color4F(1.0f, 1.0f, 0.4f, 1);
                break;
            default:
                bgColor = Color4F(0.2f, 0.2f, 0.2f, 0.8f);
                borderColor = Color4F(0.4f, 0.4f, 0.4f, 1);
                break;
        }

        auto nodeBg = DrawNode::create();
        nodeBg->drawSolidCircle(Vec2(xPos, nodeY), 55, 0, 24, bgColor);
        nodeBg->drawCircle(Vec2(xPos, nodeY), 55, 0, 24, false, borderColor);
        nodeBg->drawCircle(Vec2(xPos, nodeY), 53, 0, 24, false, borderColor);
        this->addChild(nodeBg, 0);

        // Lock icon for locked levels
        if (level.state == LevelState::LOCKED) {
            auto lockIcon = DrawNode::create();
            lockIcon->drawSolidRect(Vec2(xPos - 12, nodeY - 8), Vec2(xPos + 12, nodeY + 12),
                                    Color4F(0.4f, 0.4f, 0.5f, 1));
            lockIcon->drawSolidCircle(Vec2(xPos, nodeY + 16), 10, 0, 12,
                                      Color4F(0.4f, 0.4f, 0.5f, 0.5f));
            lockIcon->drawCircle(Vec2(xPos, nodeY + 16), 10, 0, 12, false,
                                 Color4F(0.4f, 0.4f, 0.5f, 1));
            this->addChild(lockIcon, 5);
        }

        // Add level icon inside the circle (for available and cleared)
        if (level.state != LevelState::LOCKED) {
            levelIcon->setPosition(Vec2(xPos, nodeY));
            this->addChild(levelIcon, 2);

            // Play arrow for available levels
            if (level.state == LevelState::AVAILABLE) {
                auto playArrow = DrawNode::create();
                Vec2 arrowPts[] = {Vec2(xPos + 50, nodeY - 12),
                                   Vec2(xPos + 50, nodeY + 12),
                                   Vec2(xPos + 68, nodeY)};
                playArrow->drawSolidPoly(arrowPts, 3, Color4F(1.0f, 0.84f, 0.0f, 1));
                this->addChild(playArrow, 3);
            }
        }

        // Star icons for cleared levels (draw actual star shapes)
        if (level.state == LevelState::CLEARED) {
            float starY = nodeY + 70;
            float starSpacing = 22;
            float starStartX = xPos - (level.stars > 0 ? (3 - 1) * starSpacing / 2 : 0);

            for (int s = 0; s < 3; s++) {
                float sx = xPos + (s - 1) * starSpacing;
                Color4F starColor = (s < level.stars) ?
                    Color4F(1.0f, 0.84f, 0.0f, 1) : Color4F(0.3f, 0.3f, 0.3f, 1);

                auto starDraw = DrawNode::create();
                drawStarShape(starDraw, Vec2(sx, starY), 10, starColor);
                this->addChild(starDraw, 4);
            }
        }

        // Level name label
        Color3B nameColor;
        switch (level.state) {
            case LevelState::CLEARED: nameColor = colorCleared(); break;
            case LevelState::AVAILABLE: nameColor = colorAvailable(); break;
            default: nameColor = colorLocked(); break;
        }

        auto nameLabel = Label::createWithTTF(level.name, "fonts/arial.ttf", 20);
        nameLabel->setPosition(Vec2(xPos, nodeY - 70));
        nameLabel->setColor(nameColor);
        this->addChild(nameLabel);

        // Touch event for available levels
        if (level.state == LevelState::AVAILABLE) {
            auto touchListener = EventListenerTouchOneByOne::create();
            touchListener->setSwallowTouches(true);
            int levelId = level.id;

            // Make a larger invisible touch area
            auto touchArea = Sprite::create();
            touchArea->setTextureRect(Rect(0, 0, 120, 120));
            touchArea->setColor(Color3B(0, 0, 0));
            touchArea->setOpacity(0);
            touchArea->setPosition(Vec2(xPos, nodeY));
            this->addChild(touchArea, 0, 1000 + level.id);

            touchListener->onTouchBegan = [this, levelId](Touch* touch, Event* event) {
                auto target = event->getCurrentTarget();
                Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
                Size s = target->getContentSize();
                Rect rect(0, 0, s.width, s.height);
                if (rect.containsPoint(locationInNode)) {
                    onLevelClicked(levelId);
                    return true;
                }
                return false;
            };
            _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, touchArea);
        }
    }

    // Path lines connecting level nodes
    auto pathDraw = DrawNode::create();
    for (size_t i = 0; i < levels.size() - 1; i++) {
        float x1 = startX + i * spacing;
        float x2 = startX + (i + 1) * spacing;

        Color4F pathColor = Color4F(0.4f, 0.4f, 0.5f, 0.6f);
        if (levels[i].state != LevelState::LOCKED && levels[i+1].state != LevelState::LOCKED) {
            pathColor = Color4F(0.5f, 0.8f, 0.5f, 0.8f);
        } else if (levels[i].state != LevelState::LOCKED) {
            pathColor = Color4F(0.5f, 0.5f, 0.3f, 0.7f);
        }

        pathDraw->drawLine(Vec2(x1 + 55, nodeY), Vec2(x2 - 55, nodeY), pathColor);
    }
    this->addChild(pathDraw, -1);
}

void LevelSelectScene::drawStarShape(DrawNode* draw, const Vec2& center, float size, const Color4F& color) {
    // 5-pointed star
    float outerR = size;
    float innerR = size * 0.4f;
    int points = 5;

    Vec2 vertices[10];
    for (int i = 0; i < points; i++) {
        float outerAngle = (i * 72.0f - 90.0f) * M_PI / 180.0f;
        float innerAngle = ((i * 72.0f + 36.0f) - 90.0f) * M_PI / 180.0f;
        vertices[i * 2] = Vec2(center.x + outerR * cos(outerAngle),
                                center.y + outerR * sin(outerAngle));
        vertices[i * 2 + 1] = Vec2(center.x + innerR * cos(innerAngle),
                                   center.y + innerR * sin(innerAngle));
    }
    draw->drawSolidPoly(vertices, 10, color);
}

void LevelSelectScene::onLevelClicked(int levelId) {
    auto battleScene = BattleScene::createScene(levelId);
    auto transition = TransitionFade::create(0.5f, battleScene);
    Director::getInstance()->pushScene(transition);
}

void LevelSelectScene::updatePlayerInfo() {
    auto& player = DataManager::getInstance()->getPlayer();
    _levelLabel->setString("Lv." + std::to_string(player.level));
    float expPercent = (player.exp / (float)EXP_PER_LEVEL) * 100.0f;
    _expBar->setPercentage(expPercent);
    _expLabel->setString(std::to_string(player.exp) + "/" + std::to_string(EXP_PER_LEVEL));
    _goldLabel->setString(std::to_string(player.gold));
}