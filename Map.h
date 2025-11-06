#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <vector>
#include "Wall.h"
#include "EnemyTank.h"
#include "Constant.h"

class Map {
public:
    // Hàm sinh map dựa trên level
    static void generateLevel(int level, std::vector<Wall>& walls, std::vector<EnemyTank>& enemies, int enemyNumber, SDL_Renderer* renderer);
};

#endif // MAP_H_INCLUDED
