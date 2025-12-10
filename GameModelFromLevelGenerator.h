#ifndef __GAME_MODEL_FROM_LEVEL_GENERATOR_H__
#define __GAME_MODEL_FROM_LEVEL_GENERATOR_H__

#include "models/GameModel.h"
#include "configs/models/LevelConfig.h"

class GameModelFromLevelGenerator {
public:
    static GameModel* generateGameModel(const LevelConfig& config);
};

#endif // __GAME_MODEL_FROM_LEVEL_GENERATOR_H__