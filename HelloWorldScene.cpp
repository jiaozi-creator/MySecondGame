#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"
#include <random>
#include <algorithm>
#include <functional>
#include <vector>
#include <string>

USING_NS_CC;
using namespace ui;

Scene* HelloWorld::createScene() {
    return HelloWorld::create();
}

bool HelloWorld::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _score = 100;  // 初始分数

    // 1. 添加背景
    auto background = LayerGradient::create(
        Color4B(30, 60, 30, 255),    // 开始颜色
        Color4B(10, 30, 10, 255),    // 结束颜色
        Vec2(0, 1)                   // 渐变方向
    );
    this->addChild(background, 0);

    // 添加纹理背景（可选）
    auto pattern = Sprite::create("background_pattern.png");
    if (pattern) {
        pattern->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        pattern->setOpacity(50);  // 半透明
        this->addChild(pattern, 0);
    }

    // 2. 添加标题
    auto title = Label::createWithTTF("经典纸牌游戏", "fonts/Marker Felt.ttf", 64);
    if (title) {
        title->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 80));
        title->setColor(Color3B::YELLOW);
        title->enableShadow(Color4B::BLACK, Size(2, -2), 0);
        this->addChild(title, 1);
    }

    // 3. 创建UI界面
    createUI();

    // 4. 创建示例纸牌
    createCardDemo();

    // 5. 添加关闭按钮
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem != nullptr) {
        closeItem->setPosition(Vec2(
            origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
            origin.y + closeItem->getContentSize().height / 2
        ));

        // 添加缩放动画
        auto scaleAction = Sequence::create(
            ScaleTo::create(0.5f, 1.1f),
            ScaleTo::create(0.5f, 1.0f),
            nullptr
        );
        closeItem->runAction(RepeatForever::create(scaleAction));
    }

    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 10);

    // 6. 添加说明文本
    auto instructions = Label::createWithTTF(
        "点击纸牌翻转 | 点击按钮抽牌或洗牌",
        "fonts/Marker Felt.ttf",
        24
    );
    if (instructions) {
        instructions->setPosition(Vec2(visibleSize.width / 2, 50));
        instructions->setColor(Color3B::WHITE);
        this->addChild(instructions, 1);
    }

    return true;
}

void HelloWorld::createUI() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 1. 创建分数显示
    _scoreLabel = Label::createWithTTF(
        "分数: 100",
        "fonts/Marker Felt.ttf",
        36
    );
    if (_scoreLabel) {
        _scoreLabel->setPosition(Vec2(visibleSize.width - 150, visibleSize.height - 50));
        _scoreLabel->setColor(Color3B::YELLOW);
        this->addChild(_scoreLabel, 2);
    }

    // 2. 创建控制按钮
    Size buttonSize = Size(180, 60);

    // 抽牌按钮
    auto drawButton = Button::create();
    if (drawButton) {
        drawButton->setTitleText("抽一张牌");
        drawButton->setTitleFontName("fonts/Marker Felt.ttf");
        drawButton->setTitleFontSize(24);
        drawButton->setContentSize(buttonSize);
        drawButton->setPosition(Vec2(150, visibleSize.height - 150));
        drawButton->addClickEventListener(CC_CALLBACK_1(HelloWorld::onDrawButtonClicked, this));
        drawButton->setColor(Color3B(100, 200, 255));
        this->addChild(drawButton, 3);
    }

    // 洗牌按钮
    auto shuffleButton = Button::create();
    if (shuffleButton) {
        shuffleButton->setTitleText("洗牌");
        shuffleButton->setTitleFontName("fonts/Marker Felt.ttf");
        shuffleButton->setTitleFontSize(24);
        shuffleButton->setContentSize(buttonSize);
        shuffleButton->setPosition(Vec2(150, visibleSize.height - 230));
        shuffleButton->addClickEventListener(CC_CALLBACK_1(HelloWorld::onShuffleButtonClicked, this));
        shuffleButton->setColor(Color3B(255, 200, 100));
        this->addChild(shuffleButton, 3);
    }

    // 3. 创建牌堆区域标识
    auto deckArea = DrawNode::create();
    if (deckArea) {
        deckArea->drawRect(
            Vec2(50, visibleSize.height / 2 - 100),
            Vec2(250, visibleSize.height / 2 + 100),
            Color4F(1, 1, 1, 0.3)
        );
        this->addChild(deckArea, 0);
    }

    auto deckLabel = Label::createWithTTF("牌堆", "fonts/Marker Felt.ttf", 20);
    if (deckLabel) {
        deckLabel->setPosition(Vec2(150, visibleSize.height / 2 - 120));
        deckLabel->setColor(Color3B::WHITE);
        this->addChild(deckLabel, 1);
    }
}

void HelloWorld::createCardDemo() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 1. 创建牌堆（5张牌背堆叠效果）
    float startX = 150;
    float startY = visibleSize.height / 2;

    for (int i = 0; i < 5; i++) {
        createCardBack(startX + i * 2, startY + i * 2);
    }

    // 2. 创建示例手牌（4张不同的牌）
    std::vector<std::pair<int, int>> sampleCards = {
        {1, 1},   // 红桃A
        {13, 2},  // 黑桃K
        {10, 3},  // 方块10
        {7, 4}    // 梅花7
    };

    float cardSpacing = 200;
    float cardY = visibleSize.height / 2;

    for (int i = 0; i < sampleCards.size(); i++) {
        auto card = createCompleteCard(
            sampleCards[i].first,  // 点数
            sampleCards[i].second, // 花色
            500 + i * cardSpacing, // X位置
            cardY                  // Y位置
        );

        if (card) {
            // 使用 push_back 而不是 pushBack
            _cards.push_back(card);
            this->addChild(card, 5 - i);  // 层叠效果

            // 添加入场动画
            card->setPosition(Vec2(150, cardY));
            card->runAction(Sequence::create(
                DelayTime::create(i * 0.2f),
                MoveTo::create(0.5f, Vec2(500 + i * cardSpacing, cardY)),
                nullptr
            ));
        }
    }
}

void HelloWorld::createCardBack(float x, float y) {
    auto card = Sprite::create("cards/card_general.png");
    if (!card) {
        // 如果图片加载失败，创建替代图形
        card = Sprite::create();
        auto drawNode = DrawNode::create();
        drawNode->drawSolidRect(
            Vec2(-70, -100),
            Vec2(70, 100),
            Color4F(0.1f, 0.1f, 0.5f, 1.0f)
        );
        drawNode->drawRect(
            Vec2(-70, -100),
            Vec2(70, 100),
            Color4F(1, 1, 1, 1)
        );
        card->addChild(drawNode);
    }

    card->setPosition(Vec2(x, y));
    card->setScale(0.5f);
    card->setTag(999);  // 标记为牌背

    // 添加点击事件
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch* touch, Event* event) {
        auto target = event->getCurrentTarget();
        Rect rect = Rect(
            target->getPositionX() - 70 * 0.5f,
            target->getPositionY() - 100 * 0.5f,
            140, 200
        );

        if (rect.containsPoint(touch->getLocation())) {
            // 将Node*转换为Sprite*后调用flipCard
            auto spriteTarget = dynamic_cast<Sprite*>(target);
            if (spriteTarget) {
                this->flipCard(spriteTarget);
            }
            return true;
        }
        return false;
        };

    Director::getInstance()->getEventDispatcher()
        ->addEventListenerWithSceneGraphPriority(listener, card);

    this->addChild(card, 1);
}

Sprite* HelloWorld::createCompleteCard(int number, int suit, float x, float y) {
    // 创建牌底
    auto card = Sprite::create("cards/card_general.png");
    if (!card) return nullptr;

    card->setPosition(Vec2(x, y));
    card->setScale(0.5f);
    card->setTag(number * 10 + suit);  // 组合标签

    // 花色和颜色映射
    std::string suitName;
    Color3B textColor;
    std::string colorPrefix;

    switch (suit) {
    case 1:  // 红桃
        suitName = "heart";
        textColor = Color3B::RED;
        colorPrefix = "red";
        break;
    case 2:  // 黑桃
        suitName = "spade";
        textColor = Color3B::BLACK;
        colorPrefix = "black";
        break;
    case 3:  // 方块
        suitName = "diamond";
        textColor = Color3B::RED;
        colorPrefix = "red";
        break;
    case 4:  // 梅花
        suitName = "club";
        textColor = Color3B::BLACK;
        colorPrefix = "black";
        break;
    default:
        suitName = "heart";
        textColor = Color3B::RED;
        colorPrefix = "red";
        break;
    }

    // 数字/字母映射
    std::string numberStr;
    if (number == 1) numberStr = "A";
    else if (number == 11) numberStr = "J";
    else if (number == 12) numberStr = "Q";
    else if (number == 13) numberStr = "K";
    else numberStr = std::to_string(number);

    // 尝试加载图片资源
    bool resourcesLoaded = true;
    std::string numberFile = "big_" + colorPrefix + "_" + numberStr + ".png";
    std::string suitFile = suitName + ".png";

    auto numberSprite = Sprite::create("cards/number/" + numberFile);
    auto suitSprite = Sprite::create("cards/suits/" + suitFile);

    if (!numberSprite || !suitSprite) {
        resourcesLoaded = false;
        CCLOG("资源加载失败: %s 或 %s", numberFile.c_str(), suitFile.c_str());
    }

    if (resourcesLoaded) {
        // 左上角花色
        suitSprite->setPosition(Vec2(-50, 60));
        suitSprite->setScale(0.25f);
        card->addChild(suitSprite);

        // 左上角数字
        numberSprite->setPosition(Vec2(-50, 30));
        numberSprite->setScale(0.35f);
        card->addChild(numberSprite);

        // 右下角（旋转180度）
        auto suitSprite2 = Sprite::create("cards/suits/" + suitFile);
        auto numberSprite2 = Sprite::create("cards/number/" + numberFile);

        suitSprite2->setPosition(Vec2(50, -60));
        suitSprite2->setScale(0.25f);
        suitSprite2->setRotation(180);
        card->addChild(suitSprite2);

        numberSprite2->setPosition(Vec2(50, -30));
        numberSprite2->setScale(0.35f);
        numberSprite2->setRotation(180);
        card->addChild(numberSprite2);
    }
    else {
        // 如果图片资源不存在，使用文本绘制
        std::string displayText;
        if (suit == 1) displayText = numberStr + "♥";
        else if (suit == 2) displayText = numberStr + "♠";
        else if (suit == 3) displayText = numberStr + "♦";
        else if (suit == 4) displayText = numberStr + "♣";

        auto label = Label::createWithTTF(displayText, "fonts/Marker Felt.ttf", 40);
        label->setPosition(Vec2::ZERO);
        label->setColor(textColor);
        card->addChild(label);
    }

    // 添加点击事件
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch* touch, Event* event) {
        auto target = event->getCurrentTarget();
        Rect rect = Rect(
            target->getPositionX() - 70,
            target->getPositionY() - 100,
            140, 200
        );

        if (rect.containsPoint(touch->getLocation())) {
            auto spriteTarget = dynamic_cast<Sprite*>(target);
            if (spriteTarget) {
                this->onCardClicked(spriteTarget);
            }
            return true;
        }
        return false;
        };

    Director::getInstance()->getEventDispatcher()
        ->addEventListenerWithSceneGraphPriority(listener, card);

    return card;
}

void HelloWorld::onCardClicked(Ref* sender) {
    auto card = dynamic_cast<Sprite*>(sender);
    if (!card) return;

    // 点击动画
    card->runAction(Sequence::create(
        ScaleTo::create(0.1f, 0.45f),
        ScaleTo::create(0.1f, 0.5f),
        nullptr
    ));

    // 获取卡牌信息
    int tag = card->getTag();
    if (tag == 999) {
        CCLOG("点击了牌背");
        return;
    }

    int number = tag / 10;
    int suit = tag % 10;

    std::string suitNames[] = { "红桃", "黑桃", "方块", "梅花" };
    std::string rankNames[] = { "", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

    std::string cardName = suitNames[suit - 1] + rankNames[number];

    // 显示提示信息
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto hint = Label::createWithTTF("选中: " + cardName, "fonts/Marker Felt.ttf", 28);
    if (hint) {
        hint->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 150));
        hint->setColor(Color3B::GREEN);
        this->addChild(hint, 10);

        // 2秒后消失
        hint->runAction(Sequence::create(
            DelayTime::create(1.5f),
            FadeOut::create(0.5f),
            RemoveSelf::create(),
            nullptr
        ));
    }

    // 增加分数
    _score += 5;
    updateScore(_score);
}

void HelloWorld::flipCard(Sprite* card) {
    // 翻牌动画
    card->runAction(Sequence::create(
        ScaleTo::create(0.15f, 0.05f, 0.5f),
        CallFunc::create([card]() {
            // 这里可以切换牌的纹理（牌背->正面 或 正面->牌背）
            // 例如：card->setTexture("new_texture.png");
            }),
            ScaleTo::create(0.15f, 0.5f, 0.5f),
            nullptr
            ));

    _score += 2;
    updateScore(_score);
}

void HelloWorld::onDrawButtonClicked(Ref* sender) {
    drawCard();
}

void HelloWorld::onShuffleButtonClicked(Ref* sender) {
    shuffleCards();
}

void HelloWorld::drawCard() {
    if (_cards.size() >= 7) {  // 最多显示7张手牌
        auto hint = Label::createWithTTF("手牌已满！", "fonts/Marker Felt.ttf", 24);
        hint->setPosition(Director::getInstance()->getVisibleSize() / 2);
        hint->setColor(Color3B::RED);
        this->addChild(hint, 10);

        hint->runAction(Sequence::create(
            DelayTime::create(1.0f),
            FadeOut::create(0.5f),
            RemoveSelf::create(),
            nullptr
        ));
        return;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 随机生成一张牌
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> numDist(1, 13);
    std::uniform_int_distribution<> suitDist(1, 4);

    int randomNumber = numDist(gen);
    int randomSuit = suitDist(gen);

    // 创建新牌
    float cardX = 500 + _cards.size() * 200;
    if (cardX > visibleSize.width - 150) {
        cardX = 500;
    }

    auto newCard = createCompleteCard(randomNumber, randomSuit, 150, visibleSize.height / 2);
    if (newCard) {
        newCard->setOpacity(0);  // 开始时透明

        this->addChild(newCard, 5);
        _cards.push_back(newCard);  // 使用 push_back

        // 抽牌动画
        newCard->runAction(Sequence::create(
            FadeIn::create(0.3f),
            MoveTo::create(0.5f, Vec2(cardX, visibleSize.height / 2)),
            nullptr
        ));

        // 更新分数
        _score -= 10;
        updateScore(_score);

        // 显示提示
        std::string suitNames[] = { "红桃", "黑桃", "方块", "梅花" };
        std::string rankNames[] = { "", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

        auto drawHint = Label::createWithTTF(
            "抽到: " + suitNames[randomSuit - 1] + rankNames[randomNumber],
            "fonts/Marker Felt.ttf",
            24
        );
        if (drawHint) {
            drawHint->setPosition(Vec2(visibleSize.width / 2, 100));
            drawHint->setColor(Color3B::YELLOW);
            this->addChild(drawHint, 10);

            drawHint->runAction(Sequence::create(
                DelayTime::create(1.5f),
                FadeOut::create(0.5f),
                RemoveSelf::create(),
                nullptr
            ));
        }
    }
}

void HelloWorld::shuffleCards() {
    if (_cards.empty()) return;

    // 洗牌动画
    for (auto& card : _cards) {
        card->runAction(Sequence::create(
            DelayTime::create((rand() % 10) * 0.05f),
            MoveBy::create(0.2f, Vec2(0, 50)),
            MoveBy::create(0.2f, Vec2(0, -50)),
            nullptr
        ));
    }

    // 重置分数
    _score = 100;
    updateScore(_score);

    // 显示洗牌提示
    auto shuffleHint = Label::createWithTTF("洗牌完成！", "fonts/Marker Felt.ttf", 32);
    if (shuffleHint) {
        shuffleHint->setPosition(Director::getInstance()->getVisibleSize() / 2);
        shuffleHint->setColor(Color3B(0, 255, 255));  // 使用RGB颜色替代CYAN
        this->addChild(shuffleHint, 10);

        shuffleHint->runAction(Sequence::create(
            DelayTime::create(1.0f),
            FadeOut::create(0.5f),
            RemoveSelf::create(),
            nullptr
        ));
    }
}

void HelloWorld::updateScore(int points) {
    _score = points;
    if (_score < 0) _score = 0;

    if (_scoreLabel) {
        _scoreLabel->setString("分数: " + std::to_string(_score));

        // 分数变化动画
        _scoreLabel->stopAllActions();
        _scoreLabel->setScale(1.0f);
        _scoreLabel->runAction(Sequence::create(
            ScaleTo::create(0.1f, 1.2f),
            ScaleTo::create(0.1f, 1.0f),
            nullptr
        ));

        // 根据分数改变颜色
        if (_score >= 150) {
            _scoreLabel->setColor(Color3B::GREEN);
        }
        else if (_score >= 50) {
            _scoreLabel->setColor(Color3B::YELLOW);
        }
        else {
            _scoreLabel->setColor(Color3B::RED);
        }
    }
}

void HelloWorld::menuCloseCallback(Ref* pSender) {
    // 退出前保存游戏状态
    UserDefault::getInstance()->setIntegerForKey("last_score", _score);

    Director::getInstance()->end();
}