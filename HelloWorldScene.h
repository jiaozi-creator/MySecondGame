#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>

class HelloWorld : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    // 纸牌相关方法
    void createCardBack(float x, float y);  // 创建牌背
    void createCardDemo();                   // 创建示例牌
    cocos2d::Sprite* createCompleteCard(int number, int suit, float x, float y);  // 创建完整牌

    // 游戏控制
    void shuffleCards();                     // 洗牌
    void drawCard();                         // 抽牌
    void flipCard(cocos2d::Sprite* card);    // 翻牌

    // UI方法
    void createUI();                         // 创建用户界面
    void updateScore(int points);            // 更新分数

    // 回调函数
    void menuCloseCallback(cocos2d::Ref* pSender);
    void onCardClicked(cocos2d::Ref* sender);
    void onDrawButtonClicked(cocos2d::Ref* sender);
    void onShuffleButtonClicked(cocos2d::Ref* sender);

    CREATE_FUNC(HelloWorld);

private:
    std::vector<cocos2d::Sprite*> _cards;    // 存储卡牌的容器
    cocos2d::Label* _scoreLabel;             // 分数标签
    int _score;                              // 当前分数
};

#endif // __HELLOWORLD_SCENE_H__