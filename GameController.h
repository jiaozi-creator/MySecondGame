#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "managers/UndoManager.h"
#include <functional>

class GameController : public cocos2d::Node {
private:
    GameModel* _gameModel;
    GameView* _gameView;
    UndoManager* _undoManager;

    // 初始化游戏
    bool initGame();

    // 处理卡牌点击
    void handleCardClick(int cardId);

    // 处理回退
    void handleUndo();

    // 执行手牌区替换
    void performHandReplace(CardModel* handCard);

    // 执行匹配
    void performMatch(CardModel* playfieldCard);

    // 回退手牌区替换
    void undoHandReplace(const UndoModel& undoRecord);

    // 回退匹配
    void undoMatch(const UndoModel& undoRecord);

    // 查找卡牌
    CardModel* findCardById(int cardId);

public:
    // 构造函数声明
    GameController();

    // create 方法声明
    static GameController* create();

    virtual bool init() override;

    // 开始游戏
    void startGame(int levelId = 1);

    // 获取视图
    GameView* getView() const { return _gameView; }

    // 更新
    virtual void update(float dt) override;
};

#endif // __GAME_CONTROLLER_H__