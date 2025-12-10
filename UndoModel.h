#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include "cocos2d.h"

enum UndoActionType {
    UNDO_NONE = 0,
    UNDO_HAND_REPLACE,  // 手牌区替换
    UNDO_MATCH,         // 匹配
    UNDO_DRAW           // 抽牌
};

struct UndoModel {
    UndoActionType actionType;
    int cardId1;                    // 第一个相关卡牌ID
    int cardId2;                    // 第二个相关卡牌ID
    cocos2d::Vec2 oldPosition1;     // 卡牌1的原始位置
    cocos2d::Vec2 oldPosition2;     // 卡牌2的原始位置
    bool wasFaceUp1;                // 卡牌1是否翻开
    bool wasFaceUp2;                // 卡牌2是否翻开

    UndoModel()
        : actionType(UNDO_NONE),
        cardId1(-1),
        cardId2(-1),
        wasFaceUp1(true),
        wasFaceUp2(true) {}
};

#endif // __UNDO_MODEL_H__