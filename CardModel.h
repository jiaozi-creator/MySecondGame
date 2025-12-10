#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"
#include <string>

// 花色类型
enum CardSuitType {
    CST_NONE = -1,
    CST_CLUBS,      // 梅花
    CST_DIAMONDS,   // 方块
    CST_HEARTS,     // 红桃
    CST_SPADES,     // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

// 正面类型
enum CardFaceType {
    CFT_NONE = -1,
    CFT_ACE,    // 0
    CFT_TWO,    // 1
    CFT_THREE,  // 2
    CFT_FOUR,   // 3
    CFT_FIVE,   // 4
    CFT_SIX,    // 5
    CFT_SEVEN,  // 6
    CFT_EIGHT,  // 7
    CFT_NINE,   // 8
    CFT_TEN,    // 9
    CFT_JACK,   // 10
    CFT_QUEEN,  // 11
    CFT_KING,   // 12
    CFT_NUM_CARD_FACE_TYPES
};

class CardModel {
private:
    static int nextId;
    int id;

public:
    int cardFace;
    int cardSuit;
    cocos2d::Vec2 position;
    cocos2d::Vec2 originalPosition; // 原始位置，用于回退
    bool isFaceUp;
    bool isInHand;
    bool isTopCard; // 是否为顶部牌

    CardModel(int face = CFT_ACE, int suit = CST_HEARTS,
        const cocos2d::Vec2& pos = cocos2d::Vec2::ZERO);

    // 获取唯一ID
    int getId() const { return id; }

    // 获取点数值（A=1, J=11, Q=12, K=13）
    int getFaceValue() const;

    // 获取点数显示名称
    std::string getFaceName() const;

    // 获取花色显示名称
    std::string getSuitName() const;

    // 检查是否可以匹配（点数差1）
    bool canMatchWith(const CardModel* other) const;

    // 克隆方法
    CardModel* clone() const;
};

#endif // __CARD_MODEL_H__