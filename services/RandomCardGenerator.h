#ifndef __RANDOM_CARD_GENERATOR_H__
#define __RANDOM_CARD_GENERATOR_H__

#include "configs/models/LevelConfig.h"
#include <vector>
#include <random>

class RandomCardGenerator {
private:
    static std::mt19937 rng;
    static bool initialized;

    // 初始化随机数生成器
    static void initRNG();

public:
    // 生成一个随机关卡配置
    static LevelConfig generateRandomLevel(int levelId = 1);

    // 生成一个随机卡牌
    static CardConfig generateRandomCard(bool forPlayfield = true);

    // 生成一个指定点数的随机卡牌
    static CardConfig generateCardWithFace(int face, bool forPlayfield = true);

    // 生成匹配的牌对（点数差1）
    static std::pair<CardConfig, CardConfig> generateMatchingPair();
};

#endif // __RANDOM_CARD_GENERATOR_H__
