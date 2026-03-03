#include "GameScene.h"
#include "controllers/GameController.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建游戏控制器
    auto gameController = GameController::create();
    gameController->setTag(100); // 设置tag以便查找
    this->addChild(gameController);

    // 添加重新开始按钮
    auto restartButton = ui::Button::create();
    restartButton->setTitleText("Restart");
    restartButton->setTitleFontSize(24);
    restartButton->setTitleColor(Color3B::WHITE);
    restartButton->setColor(Color3B(100, 200, 100));

    restartButton->setScale9Enabled(true);
    restartButton->setContentSize(Size(120, 50));
    restartButton->setPosition(Vec2(200, visibleSize.height - 50));

    restartButton->addClickEventListener([this](Ref* sender) {
        auto gameController = this->getChildByTag(100);
        if (gameController) {
            gameController->removeFromParent();

            // 创建新的游戏控制器
            auto newGameController = GameController::create();
            newGameController->setTag(100);
            this->addChild(newGameController);
        }
        });

    this->addChild(restartButton);

    // 添加返回按钮
    auto backButton = ui::Button::create();
    backButton->setTitleText("Back");
    backButton->setTitleFontSize(24);
    backButton->setTitleColor(Color3B::WHITE);
    backButton->setColor(Color3B(100, 100, 100));

    backButton->setScale9Enabled(true);
    backButton->setContentSize(Size(120, 50));
    backButton->setPosition(Vec2(80, visibleSize.height - 50));

    backButton->addClickEventListener([](Ref* sender) {
        Director::getInstance()->popScene();
        });

    this->addChild(backButton);

    return true;
}
