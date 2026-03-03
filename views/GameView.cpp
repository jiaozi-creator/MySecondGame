#include "GameView.h"
#include <functional>

USING_NS_CC;

GameView* GameView::create() {
    GameView* view = new GameView();
    if (view && view->init()) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool GameView::init() {
    if (!Node::init()) {
        return false;
    }

    // 设置设计分辨率适配
    auto visibleSize = Director::getInstance()->getVisibleSize();
    this->setContentSize(visibleSize);

    // 创建UI
    createUI();

    return true;
}

void GameView::createUI() {
    // 创建主牌区 (1080x1500)
    _playFieldView = Node::create();
    _playFieldView->setContentSize(Size(1080, 1500));
    _playFieldView->setPosition(0, 580); // 堆牌区在上方
    this->addChild(_playFieldView);

    // 创建堆牌区 (1080x580)
    _stackView = Node::create();
    _stackView->setContentSize(Size(1080, 580));
    _stackView->setPosition(0, 0);
    this->addChild(_stackView);

    // 创建回退按钮
    createUndoButton();
}

void GameView::createUndoButton() {
    _undoButton = ui::Button::create();
    _undoButton->setTitleText("Undo");
    _undoButton->setTitleFontSize(24);
    _undoButton->setTitleColor(Color3B::WHITE);
    _undoButton->setColor(Color3B(70, 130, 180)); // 钢蓝色

    // 创建按钮背景
    _undoButton->setScale9Enabled(true);
    _undoButton->setContentSize(Size(120, 50));
    _undoButton->setPosition(Vec2(1000, 1900)); // 右上角

    _undoButton->addClickEventListener([this](Ref* sender) {
        if (_undoCallback) {
            _undoCallback();
        }
        });

    this->addChild(_undoButton);
}

void GameView::initWithModel(const GameModel* model) {
    _gameModel = model;

    // 清空现有视图
    _playFieldView->removeAllChildren();
    _stackView->removeAllChildren();
    _cardViews.clear();

    if (!model) return;

    // 调试信息
    CCLOG("Initializing view with %zu playfield cards", model->playfieldCards.size());
    CCLOG("Initializing view with %zu hand cards", model->handCards.size());
    CCLOG("Initializing view with top hand card: %p", model->topHandCard);

    // 创建桌面牌视图 - 使用网格布局
    const int cardsPerRow = 3;
    const float cardWidth = 120;
    const float cardHeight = 180;
    const float spacingX = 40;
    const float spacingY = 40;

    // 计算起始位置，使卡牌居中
    float startX = (1080 - (cardsPerRow * cardWidth + (cardsPerRow - 1) * spacingX)) / 2;
    float startY = 1200; // 从较高位置开始

    for (size_t i = 0; i < model->playfieldCards.size(); i++) {
        auto cardModel = model->playfieldCards[i];

        // 计算行列位置
        int row = i / cardsPerRow;
        int col = i % cardsPerRow;

        float x = startX + col * (cardWidth + spacingX);
        float y = startY - row * (cardHeight + spacingY);

        CardView* cardView = CardView::createWithModel(cardModel);
        cardView->setPosition(Vec2(x, y));
        cardModel->position = Vec2(x, y); // 更新模型位置

        // 调试：输出卡牌位置信息
        CCLOG("Playfield card %d at position: (%.1f, %.1f), face: %s, suit: %s",
            cardModel->getId(), x, y,
            cardModel->getFaceName().c_str(),
            cardModel->getSuitName().c_str());

        cardView->setClickCallback([this, cardModel]() {
            CCLOG("Playfield card clicked: %s of %s (ID: %d)",
                cardModel->getFaceName().c_str(),
                cardModel->getSuitName().c_str(),
                cardModel->getId());
            if (_cardClickCallback) {
                _cardClickCallback(cardModel->getId());
            }
            });
        _playFieldView->addChild(cardView);
        _cardViews[cardModel->getId()] = cardView;
    }

    // 创建顶部手牌视图
    if (model->topHandCard) {
        CardView* topHandCardView = CardView::createWithModel(model->topHandCard);
        Vec2 handPos = Vec2(540, 290); // 堆牌区中心

        // 调试信息
        CCLOG("Top hand card: %s of %s at position: (%.1f, %.1f)",
            model->topHandCard->getFaceName().c_str(),
            model->topHandCard->getSuitName().c_str(),
            handPos.x, handPos.y);

        topHandCardView->setPosition(handPos);
        model->topHandCard->position = handPos; // 更新模型位置

        topHandCardView->setClickCallback([this, model]() {
            if (_cardClickCallback && model->topHandCard) {
                CCLOG("Top hand card clicked: %s of %s (ID: %d)",
                    model->topHandCard->getFaceName().c_str(),
                    model->topHandCard->getSuitName().c_str(),
                    model->topHandCard->getId());
                _cardClickCallback(model->topHandCard->getId());
            }
            });
        _stackView->addChild(topHandCardView);
        _cardViews[model->topHandCard->getId()] = topHandCardView;
    }

    // 创建其他手牌视图（在顶部牌下方堆叠）
    int handCardIndex = 0;
    for (auto cardModel : model->handCards) {
        if (cardModel != model->topHandCard) {
            CardView* cardView = CardView::createWithModel(cardModel);
            Vec2 stackPos = Vec2(540, 150 - handCardIndex * 25); // 堆叠显示

            // 调试信息
            CCLOG("Hand card %d at position: (%.1f, %.1f), face: %s, suit: %s",
                cardModel->getId(), stackPos.x, stackPos.y,
                cardModel->getFaceName().c_str(),
                cardModel->getSuitName().c_str());

            cardView->setPosition(stackPos);
            cardModel->position = stackPos; // 更新模型位置

            cardView->setClickCallback([this, cardModel]() {
                CCLOG("Hand card clicked: %s of %s (ID: %d)",
                    cardModel->getFaceName().c_str(),
                    cardModel->getSuitName().c_str(),
                    cardModel->getId());
                if (_cardClickCallback) {
                    _cardClickCallback(cardModel->getId());
                }
                });
            _stackView->addChild(cardView);
            _cardViews[cardModel->getId()] = cardView;
            handCardIndex++;
        }
    }
}

CardView* GameView::findCardViewById(int cardId) {
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end()) {
        return it->second;
    }
    return nullptr;
}

void GameView::setCardClickCallback(const std::function<void(int)>& callback) {
    _cardClickCallback = callback;
}

void GameView::setUndoCallback(const std::function<void()>& callback) {
    _undoCallback = callback;
}

void GameView::playCardMoveAnimation(int cardId, const Vec2& targetPos) {
    CardView* cardView = findCardViewById(cardId);
    if (cardView) {
        cardView->playMoveAnimation(targetPos);
    }
}

void GameView::playMatchAnimation(int cardId1, int cardId2) {
    // 可以在这里播放匹配特效
    CardView* cardView1 = findCardViewById(cardId1);
    CardView* cardView2 = findCardViewById(cardId2);

    if (cardView1 && cardView2) {
        // 简单的闪烁效果
        auto blink1 = Blink::create(0.5f, 2);
        auto blink2 = Blink::create(0.5f, 2);
        cardView1->runAction(blink1);
        cardView2->runAction(blink2);
    }
}

void GameView::updateView() {
    // 更新所有卡牌视图
    for (auto& pair : _cardViews) {
        pair.second->updateView();
    }
}

void GameView::addCardView(CardView* cardView) {
    if (!cardView) return;

    int cardId = cardView->getCardId();
    if (cardId >= 0) {
        _cardViews[cardId] = cardView;
    }
}
