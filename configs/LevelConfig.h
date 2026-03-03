#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "cocos2d.h"
#include <vector>

struct CardConfig {
    int cardFace;
    int cardSuit;
    cocos2d::Vec2 position;

    CardConfig() : cardFace(0), cardSuit(0) {}
    CardConfig(int face, int suit, const cocos2d::Vec2& pos)
        : cardFace(face), cardSuit(suit), position(pos) {}
};

struct LevelConfig {
    std::vector<CardConfig> playfieldCards;  // ×ÀÃæÅÆ
    std::vector<CardConfig> stackCards;      // ±¸ÓÃÅÆ¶Ñ

    LevelConfig() = default;
};

#endif // __LEVEL_CONFIG_H__
