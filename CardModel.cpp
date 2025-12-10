#include "CardModel.h"

int CardModel::nextId = 1;

CardModel::CardModel(int face, int suit, const cocos2d::Vec2& pos)
    : id(nextId++),
    cardFace(face),
    cardSuit(suit),
    position(pos),
    originalPosition(pos),
    isFaceUp(true),
    isInHand(false),
    isTopCard(false) {
}

int CardModel::getFaceValue() const {
    // A=1, 2=2, ..., J=11, Q=12, K=13
    if (cardFace == CFT_ACE) return 1;
    if (cardFace == CFT_JACK) return 11;
    if (cardFace == CFT_QUEEN) return 12;
    if (cardFace == CFT_KING) return 13;
    return cardFace + 1; // 因为CFT_TWO=1，所以需要+1
}

std::string CardModel::getFaceName() const {
    static const std::string faceNames[] = {
        "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"
    };
    if (cardFace >= 0 && cardFace < 13) {
        return faceNames[cardFace];
    }
    return "?";
}

std::string CardModel::getSuitName() const {
    static const std::string suitNames[] = {
        "♣", "♦", "♥", "♠"
    };
    if (cardSuit >= 0 && cardSuit < 4) {
        return suitNames[cardSuit];
    }
    return "?";
}

bool CardModel::canMatchWith(const CardModel* other) const {
    if (!other) return false;

    int value1 = this->getFaceValue();
    int value2 = other->getFaceValue();

    // 点数差1即可匹配
    return abs(value1 - value2) == 1;
}

CardModel* CardModel::clone() const {
    CardModel* newCard = new CardModel(cardFace, cardSuit, position);
    newCard->isFaceUp = isFaceUp;
    newCard->isInHand = isInHand;
    newCard->isTopCard = isTopCard;
    newCard->originalPosition = originalPosition;
    return newCard;
}