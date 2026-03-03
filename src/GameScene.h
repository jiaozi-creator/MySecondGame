#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class GameScene : public Scene
{
public:
    static Scene* createScene();
    virtual bool init() override;

    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__
