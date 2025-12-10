#include "CardResConfig.h"
#include <string>
#include <map>

std::map<std::string, std::string> CardResConfig::cardResources;

void CardResConfig::init() {
    // 初始化卡牌资源映射
    // 这里可以根据实际图片资源名称进行映射
}

std::string CardResConfig::getCardTexturePath(int face, int suit) {
    // 构建纹理名称，例如：card_0_1.png 表示红桃A
    return "cards/card_general.png"; // 暂时使用通用卡牌纹理
}

std::string CardResConfig::getSmallCardTexturePath(int face, int suit) {
    // 构建小卡牌纹理名称
    return "cards/card_general.png";
}

std::string CardResConfig::getBackTexturePath() {
    return "card_back.png";
}