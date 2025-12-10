#include "RandomCardGenerator.h"
#include <ctime>
#include <algorithm>
#include "cocos2d.h"  // 添加这行

USING_NS_CC;  // 添加这行，这样我们就可以直接使用 Vec2

std::mt19937 RandomCardGenerator::rng;
bool RandomCardGenerator::initialized = false;

void RandomCardGenerator::initRNG() {
    if (!initialized) {
        rng.seed(static_cast<unsigned int>(time(0)));
        initialized = true;
    }
}

LevelConfig RandomCardGenerator::generateRandomLevel(int levelId) {
    initRNG();

    LevelConfig config;

    // 根据关卡ID设置不同的参数
    int playfieldCardCount = 6 + (levelId - 1) * 2;
    if (playfieldCardCount > 12) playfieldCardCount = 12;

    int stackCardCount = 3 + (levelId - 1);
    if (stackCardCount > 8) stackCardCount = 8;

    // 生成备用牌堆（手牌区）
    for (int i = 0; i < stackCardCount; i++) {
        config.stackCards.push_back(generateRandomCard(false));
    }

    // 确保顶部手牌不是太极端（避免A或K，因为它们只能匹配一个方向）
    if (!config.stackCards.empty()) {
        int topCardFace = config.stackCards[0].cardFace;
        if (topCardFace == 0 || topCardFace == 12) {  // A或K
            // 重新生成一个中间点数的牌作为顶部牌
            std::uniform_int_distribution<int> faceDist(3, 9);
            config.stackCards[0].cardFace = faceDist(rng);
        }
    }

    // 生成桌面牌，确保至少有一些可以匹配
    int topFace = -1;
    if (!config.stackCards.empty()) {
        topFace = config.stackCards[0].cardFace;
    }

    std::uniform_int_distribution<int> faceDist(0, 12);
    std::uniform_int_distribution<int> suitDist(0, 3);
    std::uniform_real_distribution<float> xDist(100, 900);
    std::uniform_real_distribution<float> yDist(600, 1400);

    // 生成一些可以匹配的牌
    int matchingCardsCount = std::min(playfieldCardCount / 2, 3);  // 至少生成几个可以匹配的牌
    for (int i = 0; i < matchingCardsCount; i++) {
        if (topFace >= 0) {
            // 生成与顶部牌匹配的牌
            int matchingFace;
            if (topFace == 0) {
                matchingFace = 1;  // A只能匹配2
            }
            else if (topFace == 12) {
                matchingFace = 11;  // K只能匹配Q
            }
            else {
                // 随机选择+1或-1
                std::uniform_int_distribution<int> offsetDist(0, 1);
                int offset = offsetDist(rng) == 0 ? -1 : 1;
                matchingFace = topFace + offset;
                if (matchingFace < 0) matchingFace = 0;
                if (matchingFace > 12) matchingFace = 12;
            }

            config.playfieldCards.push_back(CardConfig(
                matchingFace,
                suitDist(rng),
                Vec2(xDist(rng), yDist(rng))
            ));
        }
    }

    // 生成剩余的随机牌
    int remainingCards = playfieldCardCount - matchingCardsCount;
    for (int i = 0; i < remainingCards; i++) {
        config.playfieldCards.push_back(generateRandomCard(true));
    }

    return config;
}

CardConfig RandomCardGenerator::generateRandomCard(bool forPlayfield) {
    initRNG();

    std::uniform_int_distribution<int> faceDist(0, 12);
    std::uniform_int_distribution<int> suitDist(0, 3);

    if (forPlayfield) {
        std::uniform_real_distribution<float> xDist(100, 900);
        std::uniform_real_distribution<float> yDist(600, 1400);
        return CardConfig(faceDist(rng), suitDist(rng), Vec2(xDist(rng), yDist(rng)));
    }
    else {
        return CardConfig(faceDist(rng), suitDist(rng), Vec2(0, 0));
    }
}

CardConfig RandomCardGenerator::generateCardWithFace(int face, bool forPlayfield) {
    initRNG();

    std::uniform_int_distribution<int> suitDist(0, 3);

    if (forPlayfield) {
        std::uniform_real_distribution<float> xDist(100, 900);
        std::uniform_real_distribution<float> yDist(600, 1400);
        return CardConfig(face, suitDist(rng), Vec2(xDist(rng), yDist(rng)));
    }
    else {
        return CardConfig(face, suitDist(rng), Vec2(0, 0));
    }
}

std::pair<CardConfig, CardConfig> RandomCardGenerator::generateMatchingPair() {
    initRNG();

    std::uniform_int_distribution<int> faceDist(1, 11);  // 避免A和K，因为它们只能单方向匹配
    std::uniform_int_distribution<int> suitDist1(0, 3);
    std::uniform_int_distribution<int> suitDist2(0, 3);
    std::uniform_int_distribution<int> offsetDist(0, 1);

    int face1 = faceDist(rng);
    int offset = offsetDist(rng) == 0 ? -1 : 1;
    int face2 = face1 + offset;

    // 确保face2在有效范围内
    if (face2 < 0) face2 = 1;
    if (face2 > 12) face2 = 11;

    std::uniform_real_distribution<float> xDist1(100, 500);
    std::uniform_real_distribution<float> xDist2(600, 900);
    std::uniform_real_distribution<float> yDist(600, 1400);

    return std::make_pair(
        CardConfig(face1, suitDist1(rng), Vec2(xDist1(rng), yDist(rng))),
        CardConfig(face2, suitDist2(rng), Vec2(xDist2(rng), yDist(rng)))
    );
}