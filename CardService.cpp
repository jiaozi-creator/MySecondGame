#include "CardService.h"
#include <algorithm>

bool CardService::canMatch(const CardModel* card1, const CardModel* card2) {
    if (!card1 || !card2) return false;

    // 获取点数差值（A为1，J为11，Q为12，K为13）
    int value1 = card1->getFaceValue();
    int value2 = card2->getFaceValue();

    // 点数差1即可匹配（无花色限制）
    return abs(value1 - value2) == 1;
}

bool CardService::matchCards(GameModel* model, CardModel* playfieldCard, CardModel* handCard) {
    if (!model || !playfieldCard || !handCard) return false;

    // 检查是否可以匹配
    if (!canMatch(playfieldCard, handCard)) return false;

    // 从桌面牌堆移除
    auto it = std::find(model->playfieldCards.begin(),
        model->playfieldCards.end(),
        playfieldCard);
    if (it != model->playfieldCards.end()) {
        model->playfieldCards.erase(it);
    }

    // 移动到手牌堆
    playfieldCard->isInHand = true;
    playfieldCard->isTopCard = true;

    // 更新原来的顶部牌
    if (model->topHandCard) {
        model->topHandCard->isTopCard = false;
    }

    // 更新顶部手牌
    CardModel* oldTop = model->topHandCard;
    model->topHandCard = playfieldCard;

    // 添加到手牌堆
    model->handCards.push_back(playfieldCard);

    // 如果原来的顶部牌不是playfieldCard，确保它还在手牌堆中
    if (oldTop && oldTop != playfieldCard) {
        // oldTop已经在手牌堆中，不需要重复添加
    }

    // 通知数据变化
    model->notifyDataChanged();

    return true;
}

bool CardService::replaceTopHandCard(GameModel* model, CardModel* newTopCard) {
    if (!model || !model->topHandCard || !newTopCard) return false;

    // 确保新顶部牌在手牌堆中
    auto it = std::find(model->handCards.begin(), model->handCards.end(), newTopCard);
    if (it == model->handCards.end()) {
        // 如果不在手牌堆，添加到手牌堆
        newTopCard->isInHand = true;
        model->handCards.push_back(newTopCard);
    }

    // 记录原始位置（用于动画）
    cocos2d::Vec2 oldTopPos = model->topHandCard->position;
    cocos2d::Vec2 newTopPos = newTopCard->position;

    // 更新顶部牌状态
    model->topHandCard->isTopCard = false;
    newTopCard->isTopCard = true;

    // 交换位置
    model->topHandCard->position = newTopPos;
    newTopCard->position = oldTopPos;

    // 更新顶部手牌
    CardModel* oldTop = model->topHandCard;
    model->topHandCard = newTopCard;

    // 通知数据变化
    model->notifyDataChanged();

    return true;
}

CardModel* CardService::drawFromStack(GameModel* model) {
    if (!model || model->stackCards.empty()) return nullptr;

    // 从备用牌堆取第一张牌
    CardModel* drawnCard = model->stackCards[0];
    model->stackCards.erase(model->stackCards.begin());

    return drawnCard;
}