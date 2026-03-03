#include "CardView.h"
#include <string>

USING_NS_CC;

CardView* CardView::createWithModel(CardModel* model) {
    CardView* view = new CardView();
    if (view && view->init()) {
        view->setCardModel(model);
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool CardView::init() {
    if (!Node::init()) {
        return false;
    }

    // 设置卡牌大小
    this->setContentSize(Size(120, 180));

    // 创建触摸监听器
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Size size = this->getContentSize();
        Rect rect = Rect(0, 0, size.width, size.height);

        if (rect.containsPoint(locationInNode)) {
            return true;
        }
        return false;
        };

    listener->onTouchEnded = [this](Touch* touch, Event* event) {
        if (_clickCallback) {
            _clickCallback();
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void CardView::setCardModel(CardModel* model) {
    _cardModel = model;
    createCardSprite();
    updateCardDisplay();
}

void CardView::createCardSprite() {
    // 移除旧的精灵
    if (_cardSprite) {
        _cardSprite->removeFromParent();
        _cardSprite = nullptr;
    }

    if (!_cardModel) return;

    // 根据卡牌状态创建精灵
    if (_cardModel->isFaceUp) {
        // 正面：创建完整的扑克牌
        createFaceUpCard();
    }
    else {
        // 背面：使用背面纹理
        _cardSprite = Sprite::create("card_back.png");
        if (!_cardSprite) {
            // 如果背面纹理不存在，创建蓝色矩形
            _cardSprite = Sprite::create();
            auto drawNode = DrawNode::create();
            drawNode->drawRect(Vec2(0, 0), Vec2(120, 180), Color4F(0.2f, 0.2f, 0.8f, 1.0f));
            _cardSprite->addChild(drawNode);
        }
    }

    if (_cardSprite) {
        _cardSprite->setPosition(Vec2(60, 90)); // 中心位置
        this->addChild(_cardSprite);
    }
}

void CardView::createFaceUpCard() {
    if (!_cardModel) return;

    // 创建卡牌背景（通用卡牌纹理）
    _cardSprite = Sprite::create("cards/card_general.png");
    if (!_cardSprite) {
        // 如果通用纹理不存在，创建白色矩形作为背景
        _cardSprite = Sprite::create();
        auto drawNode = DrawNode::create();
        drawNode->drawRect(Vec2(0, 0), Vec2(120, 180), Color4F(1.0f, 1.0f, 1.0f, 1.0f));
        _cardSprite->addChild(drawNode);
    }

    // 获取花色和点数名称
    std::string faceName = _cardModel->getFaceName();
    std::string suitName = _cardModel->getSuitName();

    // 确定颜色（红桃和方块为红色，梅花和黑桃为黑色）
    std::string colorPrefix = (_cardModel->cardSuit == CST_HEARTS || _cardModel->cardSuit == CST_DIAMONDS) ? "red" : "black";

    // 加载大号点数图片（左上角和右下角）
    std::string bigNumberPath = getBigNumberPath(faceName, colorPrefix);
    std::string suitPath = getSuitPath(suitName);

    // 左上角大号数字
    if (!bigNumberPath.empty()) {
        auto topLeftNumber = Sprite::create(bigNumberPath);
        if (topLeftNumber) {
            topLeftNumber->setPosition(Vec2(30, 150)); // 左上角
            topLeftNumber->setScale(0.6f);
            _cardSprite->addChild(topLeftNumber);
        }
        else {
            // 如果图片加载失败，显示文本标签
            auto label = Label::createWithSystemFont(faceName, "fonts/arial.ttf", 24);
            label->setPosition(Vec2(30, 150));
            if (colorPrefix == "red") {
                label->setColor(Color3B::RED);
            }
            else {
                label->setColor(Color3B::BLACK);
            }
            _cardSprite->addChild(label);
        }
    }

    // 右下角大号数字（旋转180度）
    if (!bigNumberPath.empty()) {
        auto bottomRightNumber = Sprite::create(bigNumberPath);
        if (bottomRightNumber) {
            bottomRightNumber->setPosition(Vec2(90, 30)); // 右下角
            bottomRightNumber->setRotation(180);
            bottomRightNumber->setScale(0.6f);
            _cardSprite->addChild(bottomRightNumber);
        }
    }

    // 花色图案（居中）
    if (!suitPath.empty()) {
        auto suitSprite = Sprite::create(suitPath);
        if (suitSprite) {
            suitSprite->setPosition(Vec2(60, 90)); // 中心
            suitSprite->setScale(1.5f);
            _cardSprite->addChild(suitSprite);
        }
        else {
            // 如果花色图片加载失败，显示文本
            auto label = Label::createWithSystemFont(suitName, "fonts/arial.ttf", 36);
            label->setPosition(Vec2(60, 90));
            if (colorPrefix == "red") {
                label->setColor(Color3B::RED);
            }
            else {
                label->setColor(Color3B::BLACK);
            }
            _cardSprite->addChild(label);
        }
    }

    // 添加边框（用于调试）
    auto border = DrawNode::create();
    border->drawRect(Vec2(0, 0), Vec2(120, 180), Color4F(0, 0, 0, 1));
    border->setLineWidth(2);
    _cardSprite->addChild(border);
}

std::string CardView::getBigNumberPath(const std::string& faceName, const std::string& color) {
    // 映射文件名
    std::string fileName;

    if (faceName == "A") fileName = "A";
    else if (faceName == "J") fileName = "J";
    else if (faceName == "Q") fileName = "Q";
    else if (faceName == "K") fileName = "K";
    else if (faceName == "10") fileName = "10";
    else fileName = faceName; // 2-9

    return "cards/number/big_" + color + "_" + fileName + ".png";
}

std::string CardView::getSmallNumberPath(const std::string& faceName, const std::string& color) {
    // 映射文件名
    std::string fileName;

    if (faceName == "A") fileName = "A";
    else if (faceName == "J") fileName = "J";
    else if (faceName == "Q") fileName = "Q";
    else if (faceName == "K") fileName = "K";
    else if (faceName == "10") fileName = "10";
    else fileName = faceName; // 2-9

    return "cards/number/small_" + color + "_" + fileName + ".png";
}

std::string CardView::getSuitPath(const std::string& suitName) {
    if (suitName == "♣") return "cards/suits/club.png";
    else if (suitName == "♦") return "cards/suits/diamond.png";
    else if (suitName == "♥") return "cards/suits/heart.png";
    else if (suitName == "♠") return "cards/suits/spade.png";
    return "";
}

void CardView::updateCardDisplay() {
    if (!_cardModel) return;

    // 更新位置
    this->setPosition(_cardModel->position);

    // 重新创建卡牌精灵（确保正面/背面正确）
    createCardSprite();
}

void CardView::setClickCallback(const std::function<void()>& callback) {
    _clickCallback = callback;
}

void CardView::playMoveAnimation(const Vec2& targetPos, float duration) {
    auto moveTo = MoveTo::create(duration, targetPos);
    auto easeOut = EaseBackOut::create(moveTo);
    this->runAction(easeOut);

    // 更新模型位置
    if (_cardModel) {
        _cardModel->position = targetPos;
    }
}

void CardView::updateView() {
    // 更新位置
    if (_cardModel) {
        this->setPosition(_cardModel->position);
    }

    // 更新显示
    updateCardDisplay();
}

int CardView::getCardId() const {
    return _cardModel ? _cardModel->getId() : -1;
}
