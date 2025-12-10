#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include <vector>
#include <functional>
#include "CardModel.h"
#include "UndoModel.h"

class GameModel {
private:
    std::vector<std::function<void()>> dataChangedListeners;

public:
    std::vector<CardModel*> playfieldCards;  // 桌面牌
    std::vector<CardModel*> stackCards;      // 备用牌堆
    std::vector<CardModel*> handCards;       // 手牌区所有牌
    CardModel* topHandCard;                  // 手牌区顶部牌

    std::vector<UndoModel> undoHistory;      // 回退历史

    GameModel();
    ~GameModel();

    // 查找卡牌
    CardModel* findCardById(int cardId);
    CardModel* findCardInPlayfield(int cardId);
    CardModel* findCardInHand(int cardId);
    CardModel* findCardInStack(int cardId);

    // 数据监听
    void addDataChangedListener(const std::function<void()>& listener);
    void notifyDataChanged();

    // 游戏状态检查
    bool hasMovableCards() const;
    bool isGameOver() const;

    // 清理
    void clear();
};

#endif // __GAME_MODEL_H__