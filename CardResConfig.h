#ifndef __CARD_RES_CONFIG_H__
#define __CARD_RES_CONFIG_H__

#include <string>
#include <map>
#include "cocos2d.h"

class CardResConfig {
private:
    static std::map<std::string, std::string> cardResources;

public:
    static void init();
    static std::string getCardTexturePath(int face, int suit);
    static std::string getSmallCardTexturePath(int face, int suit);
    static std::string getBackTexturePath();
};

#endif // __CARD_RES_CONFIG_H__