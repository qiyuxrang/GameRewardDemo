#include "UI/IconFactory.h"
#include "Managers/DataManager.h"

USING_NS_CC;

Node* IconFactory::createItemIcon(ItemType type, float size) {
    auto draw = DrawNode::create();
    float r = size / 2;

    switch (type) {
        case ItemType::GOLD: {
            // Gold coin: filled circle + inner ring + shine
            draw->drawSolidCircle(Vec2(0, 0), r, 0.0f, 32, Color4F(1.0f, 0.84f, 0.0f, 1.0f));
            draw->drawCircle(Vec2(0, 0), r * 0.7f, 0.0f, 32, false, Color4F(0.85f, 0.65f, 0.0f, 1.0f));
            draw->drawSolidCircle(Vec2(r * 0.2f, r * 0.2f), r * 0.15f, 0.0f, 8,
                                  Color4F(1.0f, 0.95f, 0.5f, 0.8f));
            auto label = Label::createWithTTF("$", "fonts/arial.ttf", size * 0.35f);
            label->setColor(Color3B(180, 130, 0));
            draw->addChild(label);
            break;
        }
        case ItemType::EXP_ORB: {
            // Exp orb: glowing sphere with rings
            draw->drawSolidCircle(Vec2(0, 0), r, 0.0f, 32, Color4F(0.4f, 1.0f, 0.4f, 1.0f));
            draw->drawSolidCircle(Vec2(0, 0), r * 0.6f, 0.0f, 32, Color4F(0.7f, 1.0f, 0.7f, 1.0f));
            draw->drawSolidCircle(Vec2(-r * 0.1f, r * 0.15f), r * 0.2f, 0.0f, 12,
                                  Color4F(1.0f, 1.0f, 0.9f, 0.7f));
            draw->drawCircle(Vec2(0, 0), r * 0.85f, 0.0f, 32, false, Color4F(0.2f, 0.7f, 0.2f, 0.5f));
            break;
        }
        case ItemType::WEAPON: {
            // Sword: blade + handle + guard
            float bladeLen = r * 1.4f;
            float bladeW = r * 0.25f;
            float handleLen = r * 0.6f;
            // Blade
            Vec2 blade[] = {
                Vec2(-bladeW, -handleLen),
                Vec2(0, -handleLen - bladeLen),
                Vec2(bladeW, -handleLen)
            };
            draw->drawSolidPoly(blade, 3, Color4F(0.7f, 0.7f, 0.8f, 1.0f));
            // Guard (cross bar)
            draw->drawSolidRect(Vec2(-r * 0.6f, -handleLen), Vec2(r * 0.6f, -handleLen + r * 0.1f),
                                Color4F(0.6f, 0.5f, 0.2f, 1.0f));
            // Handle
            draw->drawSolidRect(Vec2(-r * 0.12f, -handleLen + r * 0.1f), Vec2(r * 0.12f, r * 0.2f),
                                Color4F(0.55f, 0.27f, 0.07f, 1.0f));
            // Pommel
            draw->drawSolidCircle(Vec2(0, r * 0.25f), r * 0.15f, 0.0f, 12,
                                  Color4F(0.6f, 0.5f, 0.2f, 1.0f));
            break;
        }
        case ItemType::ARMOR: {
            // Shield: shield shape (rounded bottom)
            draw->drawSolidCircle(Vec2(0, r * 0.15f), r * 0.85f, 0.0f, 16, Color4F(0.55f, 0.27f, 0.07f, 1.0f));
            // Shield emblem: cross
            draw->drawSolidRect(Vec2(-r * 0.08f, -r * 0.3f), Vec2(r * 0.08f, r * 0.5f),
                                Color4F(0.8f, 0.6f, 0.2f, 1.0f));
            draw->drawSolidRect(Vec2(-r * 0.35f, r * 0.05f), Vec2(r * 0.35f, r * 0.2f),
                                Color4F(0.8f, 0.6f, 0.2f, 1.0f));
            // Border
            draw->drawCircle(Vec2(0, r * 0.15f), r * 0.85f, 0.0f, 16, false,
                             Color4F(0.3f, 0.15f, 0.0f, 1.0f));
            break;
        }
        case ItemType::POTION: {
            // Potion bottle: neck + body
            // Bottle body (ellipse approx with circle)
            draw->drawSolidCircle(Vec2(0, -r * 0.15f), r * 0.65f, 0.0f, 20,
                                  Color4F(1.0f, 0.4f, 0.4f, 1.0f));
            // Bottle neck
            draw->drawSolidRect(Vec2(-r * 0.15f, r * 0.3f), Vec2(r * 0.15f, r * 0.8f),
                                Color4F(0.9f, 0.9f, 0.9f, 0.8f));
            // Bottle cap
            draw->drawSolidRect(Vec2(-r * 0.2f, r * 0.8f), Vec2(r * 0.2f, r * 0.95f),
                                Color4F(0.6f, 0.3f, 0.1f, 1.0f));
            // Shine
            draw->drawSolidCircle(Vec2(-r * 0.15f, -r * 0.2f), r * 0.15f, 0.0f, 8,
                                  Color4F(1.0f, 0.7f, 0.7f, 0.5f));
            break;
        }
        case ItemType::KEY: {
            // Key: shaft + teeth + head (circle)
            // Shaft
            draw->drawSolidRect(Vec2(-r * 0.1f, -r * 0.1f), Vec2(r * 0.85f, r * 0.1f),
                                Color4F(1.0f, 0.84f, 0.0f, 1.0f));
            // Key head (ring)
            draw->drawSolidCircle(Vec2(-r * 0.45f, 0), r * 0.4f, 0.0f, 16,
                                  Color4F(1.0f, 0.84f, 0.0f, 1.0f));
            draw->drawSolidCircle(Vec2(-r * 0.45f, 0), r * 0.2f, 0.0f, 16,
                                  Color4F(0.5f, 0.3f, 0.0f, 1.0f));
            // Teeth (two prongs)
            draw->drawSolidRect(Vec2(r * 0.55f, -r * 0.1f), Vec2(r * 0.75f, -r * 0.35f),
                                Color4F(1.0f, 0.84f, 0.0f, 1.0f));
            draw->drawSolidRect(Vec2(r * 0.35f, -r * 0.1f), Vec2(r * 0.55f, -r * 0.25f),
                                Color4F(1.0f, 0.84f, 0.0f, 1.0f));
            break;
        }
        default: {
            draw->drawSolidCircle(Vec2(0, 0), r, 0.0f, 32, Color4F(0.5f, 0.5f, 0.5f, 1.0f));
            break;
        }
    }

    return draw;
}

Color3B IconFactory::parseHexColor(const std::string& hex) {
    if (hex.length() >= 7 && hex[0] == '#') {
        int r = std::stoi(hex.substr(1, 2), nullptr, 16);
        int g = std::stoi(hex.substr(3, 2), nullptr, 16);
        int b = std::stoi(hex.substr(5, 2), nullptr, 16);
        return Color3B(r, g, b);
    }
    return Color3B(200, 200, 200);
}