#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include "cocos2d.h"

// Design resolution
static const float DESIGN_WIDTH = 1280.0f;
static const float DESIGN_HEIGHT = 720.0f;

// Animation duration
static const float FLY_DURATION = 0.8f;
static const float FLY_INTERVAL = 0.2f;
static const float BATTLE_DURATION = 5.0f;
static const float LEVEL_UP_FLASH = 0.5f;
static const float NUM_POP_DURATION = 0.3f;

// Exp & level
static const int EXP_PER_LEVEL = 100;

// Inventory capacity
static const int INVENTORY_MAX_SLOTS = 8;

// Item type enum (scoped to avoid name conflict)
enum class ItemType {
    GOLD = 0,
    EXP_ORB,
    WEAPON,
    ARMOR,
    POTION,
    KEY,
    COUNT
};

// Level state enum
enum class LevelState {
    LOCKED = 0,
    AVAILABLE,
    CLEARED
};

// Color constants - use inline functions to avoid static const issues with MSVC
inline cocos2d::Color3B colorGold()    { return cocos2d::Color3B(255, 215, 0); }
inline cocos2d::Color3B colorExpGreen() { return cocos2d::Color3B(100, 255, 100); }
inline cocos2d::Color3B colorNormal()   { return cocos2d::Color3B(200, 200, 200); }
inline cocos2d::Color3B colorLocked()   { return cocos2d::Color3B(100, 100, 100); }
inline cocos2d::Color3B colorAvailable(){ return cocos2d::Color3B(255, 255, 100); }
inline cocos2d::Color3B colorCleared()  { return cocos2d::Color3B(100, 200, 100); }

#endif