#include "AppDelegate.h"
#include "Scenes/LevelSelectScene.h"
#include "Constants.h"

USING_NS_CC;

AppDelegate::AppDelegate() {}
AppDelegate::~AppDelegate() {}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = GLViewImpl::create("GameRewardDemo");
        director->setOpenGLView(glview);
    }

    glview->setDesignResolutionSize(DESIGN_WIDTH, DESIGN_HEIGHT, ResolutionPolicy::FIXED_HEIGHT);

    director->setDisplayStats(true);
    director->setAnimationInterval(1.0f / 60.0f);

    auto scene = LevelSelectScene::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {}
void AppDelegate::applicationWillEnterForeground() {}