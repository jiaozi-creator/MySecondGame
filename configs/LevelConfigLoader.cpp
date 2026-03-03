#include "LevelConfigLoader.h"
#include "services/RandomCardGenerator.h"

USING_NS_CC;

LevelConfig LevelConfigLoader::loadLevelConfig(int levelId) {
    // 使用改进的随机生成器
    return RandomCardGenerator::generateRandomLevel(levelId);

    // 如果想保留固定配置用于测试，可以这样：
    // return generateFixedLevel(); // 你可以创建一个固定配置的函数
}

LevelConfig LevelConfigLoader::loadLevelFromJson(const std::string& jsonContent) {
    // 如果需要从JSON加载，这里可以解析JSON
    // 为了简化，我们返回随机配置
    return loadLevelConfig(1);
}
