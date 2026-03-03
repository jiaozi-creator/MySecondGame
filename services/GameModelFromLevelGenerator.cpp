#include "GameModelFromLevelGenerator.h"
#include "models/CardModel.h"

USING_NS_CC;

GameModel* GameModelFromLevelGenerator::generateGameModel(const LevelConfig& config) {
    GameModel* gameModel = new GameModel();

    // 生成桌面牌
    for (const auto& cardConfig : config.playfieldCards) {
        CardModel* card = new CardModel(cardConfig.cardFace,
            cardConfig.cardSuit,
            cardConfig.position);
        card->isInHand = false;
        gameModel->playfieldCards.push_back(card);
    }

    // 生成备用牌堆
    for (const auto& cardConfig : config.stackCards) {
        CardModel* card = new CardModel(cardConfig.cardFace,
            cardConfig.cardSuit,
            Vec2(540, 290)); // 堆牌区中心
        card->isInHand = true;
        card->isTopCard = false;
        gameModel->stackCards.push_back(card);
    }

    // 设置顶部手牌（从备用牌堆取第一张）
    if (!gameModel->stackCards.empty()) {
        gameModel->topHandCard = gameModel->stackCards[0];
        gameModel->topHandCard->isTopCard = true;
    }

    return gameModel;
}
