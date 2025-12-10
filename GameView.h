#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "models/GameModel.h"
#include "CardView.h"
#include <functional>
#include <map>

class GameView : public cocos2d::Node {
private:
    const GameModel* _gameModel;
    cocos2d::Node* _playFieldView;      // 主牌区
    cocos2d::Node* _stackView;          // 堆牌区
    cocos2d::ui::Button* _undoButton;   // 回退按钮

    std::map<int, CardView*> _cardViews; // 卡牌ID到视图的映射

    std::function<void(int)> _cardClickCallback;
    std::function<void()> _undoCallback;

    void createUI();
    void createUndoButton();
    CardView* findCardViewById(int cardId);

public:
    static GameView* create();
    virtual bool init() override;

    void initWithModel(const GameModel* model);

    // 设置回调
    void setCardClickCallback(const std::function<void(int)>& callback);
    void setUndoCallback(const std::function<void()>& callback);

    // 播放动画
    void playCardMoveAnimation(int cardId, const cocos2d::Vec2& targetPos);
    void playMatchAnimation(int cardId1, int cardId2);

    // 更新视图
    void updateView();

    // 获取视图节点
    cocos2d::Node* getPlayFieldView() const { return _playFieldView; }
    cocos2d::Node* getStackView() const { return _stackView; }

    // 添加卡牌视图
    void addCardView(CardView* cardView);
};

#endif // __GAME_VIEW_H__