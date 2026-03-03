#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "models/CardModel.h"
#include <functional>
#include <string>

class CardView : public cocos2d::Node {
private:
    CardModel* _cardModel;
    cocos2d::Sprite* _cardSprite;
    std::function<void()> _clickCallback;

    void createCardSprite();
    void createFaceUpCard();
    void updateCardDisplay();

    // 辅助方法：获取资源路径
    std::string getBigNumberPath(const std::string& faceName, const std::string& color);
    std::string getSmallNumberPath(const std::string& faceName, const std::string& color);
    std::string getSuitPath(const std::string& suitName);

public:
    static CardView* createWithModel(CardModel* model);
    virtual bool init() override;

    void setCardModel(CardModel* model);
    CardModel* getCardModel() const { return _cardModel; }

    void setClickCallback(const std::function<void()>& callback);

    // 播放移动动画
    void playMoveAnimation(const cocos2d::Vec2& targetPos, float duration = 0.3f);

    // 更新显示
    void updateView();

    // 获取卡牌ID
    int getCardId() const;
};

#endif // __CARD_VIEW_H__
