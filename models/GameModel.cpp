#include "GameModel.h"
#include <algorithm>
#include "models/CardModel.h"
#include "models/UndoModel.h"
GameModel::GameModel() : topHandCard(nullptr) {
}

GameModel::~GameModel() {
    clear();
}

CardModel* GameModel::findCardById(int cardId) {
    // 在桌面牌中查找
    for (auto card : playfieldCards) {
        if (card->getId() == cardId) return card;
    }

    // 在手牌中查找
    for (auto card : handCards) {
        if (card->getId() == cardId) return card;
    }

    // 在备用牌堆中查找
    for (auto card : stackCards) {
        if (card->getId() == cardId) return card;
    }

    return nullptr;
}

CardModel* GameModel::findCardInPlayfield(int cardId) {
    for (auto card : playfieldCards) {
        if (card->getId() == cardId) return card;
    }
    return nullptr;
}

CardModel* GameModel::findCardInHand(int cardId) {
    for (auto card : handCards) {
        if (card->getId() == cardId) return card;
    }
    return nullptr;
}

CardModel* GameModel::findCardInStack(int cardId) {
    for (auto card : stackCards) {
        if (card->getId() == cardId) return card;
    }
    return nullptr;
}

void GameModel::addDataChangedListener(const std::function<void()>& listener) {
    dataChangedListeners.push_back(listener);
}

void GameModel::notifyDataChanged() {
    for (auto& listener : dataChangedListeners) {
        listener();
    }
}

bool GameModel::hasMovableCards() const {
    if (!topHandCard) return false;

    // 检查是否有可移动的桌面牌
    for (auto card : playfieldCards) {
        if (card->canMatchWith(topHandCard)) {
            return true;
        }
    }

    return false;
}

bool GameModel::isGameOver() const {
    // 游戏结束条件：没有可移动的牌且备用牌堆为空
    return !hasMovableCards() && stackCards.empty();
}

void GameModel::clear() {
    // 清理内存
    for (auto card : playfieldCards) delete card;
    for (auto card : handCards) delete card;
    for (auto card : stackCards) delete card;

    playfieldCards.clear();
    handCards.clear();
    stackCards.clear();
    topHandCard = nullptr;
    undoHistory.clear();
    dataChangedListeners.clear();
}
