#ifndef __CARD_SERVICE_H__
#define __CARD_SERVICE_H__

#include "models/CardModel.h"
#include "models/GameModel.h"

class CardService {
public:
    // 检查两张牌是否可以匹配
    static bool canMatch(const CardModel* card1, const CardModel* card2);

    // 执行匹配操作
    static bool matchCards(GameModel* model, CardModel* playfieldCard, CardModel* handCard);

    // 执行手牌区替换
    static bool replaceTopHandCard(GameModel* model, CardModel* newTopCard);

    // 从备用牌堆抽牌
    static CardModel* drawFromStack(GameModel* model);
};

#endif // __CARD_SERVICE_H__
