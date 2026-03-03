#include "GameController.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "services/GameModelFromLevelGenerator.h"
#include "services/CardService.h"

USING_NS_CC;

// 构造函数实现
GameController::GameController()
    : _gameModel(nullptr)
    , _gameView(nullptr)
    , _undoManager(nullptr)
{
}

// create 方法实现
GameController* GameController::create()
{
    GameController* controller = new GameController();
    if (controller && controller->init())
    {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

bool GameController::init()
{
    if (!Node::init())
    {
        return false;
    }

    // 初始化管理器
    _undoManager = new UndoManager();

    // 创建视图
    _gameView = GameView::create();
    this->addChild(_gameView);

    // 设置视图回调
    _gameView->setCardClickCallback([this](int cardId) {
        this->handleCardClick(cardId);
        });

    _gameView->setUndoCallback([this]() {
        this->handleUndo();
        });

    // 初始化游戏
    return initGame();
}

bool GameController::initGame()
{
    // 清理旧数据
    if (_gameModel) {
        delete _gameModel;
        _gameModel = nullptr;
    }

    // 创建新游戏
    startGame(1);

    return true;
}

void GameController::startGame(int levelId)
{
    CCLOG("Starting game with level %d", levelId);

    // 加载关卡配置
    LevelConfig levelConfig = LevelConfigLoader::loadLevelConfig(levelId);
    CCLOG("Loaded %zu playfield cards", levelConfig.playfieldCards.size());
    CCLOG("Loaded %zu stack cards", levelConfig.stackCards.size());

    // 生成游戏模型
    _gameModel = GameModelFromLevelGenerator::generateGameModel(levelConfig);
    CCLOG("Generated %zu playfield cards", _gameModel->playfieldCards.size());
    CCLOG("Generated %zu stack cards", _gameModel->stackCards.size());

    if (_gameModel->topHandCard) {
        CCLOG("Top hand card: %s of %s",
            _gameModel->topHandCard->getFaceName().c_str(),
            _gameModel->topHandCard->getSuitName().c_str());
    }

    // 设置数据变化监听
    _gameModel->addDataChangedListener([this]() {
        // 数据变化时更新视图
        if (_gameView) {
            _gameView->updateView();
        }
        });

    // 初始化视图
    if (_gameView) {
        _gameView->initWithModel(_gameModel);
    }

    // 清空回退历史
    if (_undoManager) {
        _undoManager->clear();
    }
}

void GameController::handleCardClick(int cardId)
{
    if (!_gameModel) {
        CCLOG("GameModel is null");
        return;
    }

    CardModel* clickedCard = findCardById(cardId);
    if (!clickedCard) {
        CCLOG("Card with ID %d not found", cardId);
        return;
    }

    CCLOG("Card clicked: %s of %s (ID: %d, isInHand: %d)",
        clickedCard->getFaceName().c_str(),
        clickedCard->getSuitName().c_str(),
        cardId,
        clickedCard->isInHand);

    // 判断点击的是手牌还是桌面牌
    if (clickedCard->isInHand) {
        // 需求1：手牌区翻牌替换
        CCLOG("Performing hand replace");
        performHandReplace(clickedCard);
    }
    else {
        // 需求2：桌面牌和手牌区顶部牌匹配
        CCLOG("Attempting match with top hand card");
        performMatch(clickedCard);
    }
}

void GameController::performHandReplace(CardModel* handCard)
{
    if (!_gameModel || !_gameModel->topHandCard || !handCard) {
        CCLOG("Cannot perform hand replace: gameModel=%p, topHandCard=%p, handCard=%p",
            _gameModel, _gameModel ? _gameModel->topHandCard : nullptr, handCard);
        return;
    }

    // 记录回退操作
    UndoModel undoRecord;
    undoRecord.actionType = UNDO_HAND_REPLACE;
    undoRecord.cardId1 = handCard->getId();
    undoRecord.cardId2 = _gameModel->topHandCard->getId();
    undoRecord.oldPosition1 = handCard->position;
    undoRecord.oldPosition2 = _gameModel->topHandCard->position;
    undoRecord.wasFaceUp1 = handCard->isFaceUp;
    undoRecord.wasFaceUp2 = _gameModel->topHandCard->isFaceUp;

    _undoManager->recordUndo(undoRecord);

    // 保存原始位置（用于动画）
    Vec2 targetPos = _gameModel->topHandCard->position;

    // 执行替换
    CardService::replaceTopHandCard(_gameModel, handCard);

    // 播放移动动画
    if (_gameView) {
        _gameView->playCardMoveAnimation(handCard->getId(), targetPos);
    }
}

void GameController::performMatch(CardModel* playfieldCard)
{
    if (!_gameModel || !_gameModel->topHandCard || !playfieldCard) {
        CCLOG("Cannot perform match: gameModel=%p, topHandCard=%p, playfieldCard=%p",
            _gameModel, _gameModel ? _gameModel->topHandCard : nullptr, playfieldCard);
        return;
    }

    // 检查是否可以匹配
    if (!CardService::canMatch(playfieldCard, _gameModel->topHandCard)) {
        CCLOG("Cannot match: %s with %s",
            playfieldCard->getFaceName().c_str(),
            _gameModel->topHandCard->getFaceName().c_str());
        return;
    }

    // 记录回退操作
    UndoModel undoRecord;
    undoRecord.actionType = UNDO_MATCH;
    undoRecord.cardId1 = playfieldCard->getId();
    undoRecord.cardId2 = _gameModel->topHandCard->getId();
    undoRecord.oldPosition1 = playfieldCard->position;
    undoRecord.oldPosition2 = _gameModel->topHandCard->position;
    undoRecord.wasFaceUp1 = playfieldCard->isFaceUp;
    undoRecord.wasFaceUp2 = _gameModel->topHandCard->isFaceUp;

    _undoManager->recordUndo(undoRecord);

    // 保存目标位置（手牌区顶部位置）
    Vec2 targetPos = _gameModel->topHandCard->position;

    // 执行匹配
    CardService::matchCards(_gameModel, playfieldCard, _gameModel->topHandCard);

    // 播放移动动画和匹配特效
    if (_gameView) {
        _gameView->playCardMoveAnimation(playfieldCard->getId(), targetPos);
        _gameView->playMatchAnimation(playfieldCard->getId(), _gameModel->topHandCard->getId());
    }
}

void GameController::handleUndo()
{
    // 需求3：回退功能
    if (!_undoManager || !_undoManager->canUndo() || !_gameModel) {
        CCLOG("Cannot undo: undoManager=%p, canUndo=%d, gameModel=%p",
            _undoManager, _undoManager ? _undoManager->canUndo() : 0, _gameModel);
        return;
    }

    UndoModel lastUndo = _undoManager->getLastUndo();
    CCLOG("Undoing action type: %d, card1: %d, card2: %d",
        lastUndo.actionType, lastUndo.cardId1, lastUndo.cardId2);

    switch (lastUndo.actionType) {
    case UNDO_HAND_REPLACE:
        undoHandReplace(lastUndo);
        break;

    case UNDO_MATCH:
        undoMatch(lastUndo);
        break;

    default:
        CCLOG("Unknown undo action type: %d", lastUndo.actionType);
        break;
    }

    // 弹出回退记录
    _undoManager->popLastUndo();

    // 更新视图
    if (_gameView) {
        _gameView->updateView();
    }

    CCLOG("Undo completed. Remaining undo records: %zu", _undoManager->getHistoryCount());
}

void GameController::undoHandReplace(const UndoModel& undoRecord)
{
    CardModel* card1 = findCardById(undoRecord.cardId1);
    CardModel* card2 = findCardById(undoRecord.cardId2);

    if (!card1 || !card2) {
        CCLOG("Cannot undo hand replace: card1=%p, card2=%p", card1, card2);
        return;
    }

    // 恢复位置
    card1->position = undoRecord.oldPosition1;
    card2->position = undoRecord.oldPosition2;

    // 恢复顶部牌
    _gameModel->topHandCard = card2;

    // 恢复状态
    card1->isTopCard = false;
    card2->isTopCard = true;

    // 播放回移动画
    if (_gameView) {
        _gameView->playCardMoveAnimation(card1->getId(), undoRecord.oldPosition1);
        _gameView->playCardMoveAnimation(card2->getId(), undoRecord.oldPosition2);
    }
}

void GameController::undoMatch(const UndoModel& undoRecord)
{
    CardModel* card1 = findCardById(undoRecord.cardId1); // 桌面牌
    CardModel* card2 = findCardById(undoRecord.cardId2); // 原顶部手牌

    if (!card1 || !card2) {
        CCLOG("Cannot undo match: card1=%p, card2=%p", card1, card2);
        return;
    }

    // 恢复桌面牌
    card1->position = undoRecord.oldPosition1;
    card1->isInHand = false;
    card1->isTopCard = false;

    // 添加到桌面牌堆
    _gameModel->playfieldCards.push_back(card1);

    // 从手牌堆移除
    auto it = std::find(_gameModel->handCards.begin(),
        _gameModel->handCards.end(),
        card1);
    if (it != _gameModel->handCards.end()) {
        _gameModel->handCards.erase(it);
    }

    // 恢复原顶部手牌
    _gameModel->topHandCard = card2;
    card2->isTopCard = true;

    // 播放回移动画
    if (_gameView) {
        _gameView->playCardMoveAnimation(card1->getId(), undoRecord.oldPosition1);
        _gameView->playCardMoveAnimation(card2->getId(), undoRecord.oldPosition2);
    }
}

CardModel* GameController::findCardById(int cardId)
{
    if (!_gameModel) return nullptr;
    return _gameModel->findCardById(cardId);
}

void GameController::update(float dt)
{
    Node::update(dt);

    // 可以在这里更新游戏状态
    if (_gameModel && _gameModel->isGameOver()) {
        CCLOG("Game Over!");
    }
}
