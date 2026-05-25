# GameRewardDemo

A Cocos2d-x 4.0 C++ demo project showcasing mobile game reward and upgrade systems — built as a portfolio piece for game client developer positions.

## Game Flow

```
Level Select → Battle (auto-combat) → Victory Result → Star Fly-in → Reward Fly-in → Level Up → Unlock Next Level
```

## Features

- **Level Select** — 5 themed levels with lock/unlock/progress states, player info panel with exp bar
- **Auto Battle** — Countdown timer with tap-to-speed-up, enemy hit feedback
- **Victory Result** — Stars fly from panel to top via bezier arc, then reward items fly to inventory bar
- **Reward Fly-in Animation** — Bezier curve arc trajectory with pop-out, shrink-in-flight, arrival bounce
- **Inventory Bar** — Bottom slot bar with icon display and count bounce animation
- **Level Up Effect** — Full-screen flash, "LEVEL UP!" text with glow, attribute changes, gold particles
- **All DrawNode UI** — No sprite assets required; all icons and shapes drawn procedurally

## Quick Start

### Prerequisites

- Windows 10+
- Visual Studio 2019/2022 (with C++ Desktop Development workload)
- CMake 3.16+
- Git

### One-Click Setup

Right-click `setup.bat` → **Run as administrator**

The script will automatically:
1. Detect Visual Studio and CMake
2. Clone cocos2d-x v4 engine (shallow clone)
3. Generate build files and compile Release

### Manual Setup

```bash
git clone https://github.com/qiyuxrang/GameRewardDemo.git
cd GameRewardDemo
git clone --depth 1 -b v4 https://github.com/cocos2d/cocos2d-x.git cocos2d
mkdir build && cd build
cmake -G "Visual Studio 17 2022" -A Win32 ..
cmake --build . --config Release
```

Run: `build\bin\GameRewardDemo\Release\GameRewardDemo.exe`

## Project Structure

```
Classes/
├── AppDelegate.cpp/h
├── Constants.h                  # Game constants, colors, enums
├── Models/
│   ├── ItemData.h               # Item definition (GOLD, EXP, WEAPON, ARMOR, POTION, KEY)
│   ├── LevelData.h              # Level definition with rewards
│   └── PlayerData.h             # Player state (level, exp, gold, inventory)
├── Managers/
│   └── DataManager.cpp/h        # Singleton: levels, player, item defs
├── Scenes/
│   ├── LevelSelectScene.cpp/h   # Level nodes with themed icons
│   ├── BattleScene.cpp/h        # Auto-combat with progress bar
│   └── ResultScene.cpp/h        # Victory: star fly-in → reward panel → collect
├── Layers/
│   ├── RewardFlyLayer.cpp/h     # Bezier arc fly-in animation
│   ├── InventoryBarLayer.cpp/h  # Bottom inventory bar
│   └── LevelUpEffectLayer.cpp/h # Level up celebration effect
└── UI/
    ├── RewardItemNode.cpp/h     # Single reward item display
    ├── AttributePanel.cpp/h     # Player attribute panel
    └── IconFactory.cpp/h        # Procedural DrawNode icon generation
```

## Key Cocos2d-x Techniques

| Technique | API Used |
|-----------|----------|
| Arc fly-in trajectory | `BezierTo` + `ccBezierConfig` |
| Star pop-out + bounce | `EaseBackOut` + `EaseBounceOut` |
| Staggered animation | `Sequence` + `DelayTime` + `CallFunc` |
| Progress bar | `ProgressTimer` (Type::BAR) |
| Text glow | `Label::enableGlow()` |
| Level up flash | `LayerColor` + `FadeIn/FadeOut` |
| Procedural icons | `DrawNode::drawSolidPoly/Circle/Rect` |
| Scene transitions | `TransitionFade` |

## License

MIT
