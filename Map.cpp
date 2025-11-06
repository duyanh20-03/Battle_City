#include "Map.h"
#include <cstdlib>

void Map::generateLevel(int level, std::vector<Wall>& walls, std::vector<EnemyTank>& enemies, int enemyNumber, SDL_Renderer* renderer) {
    walls.clear();
    enemies.clear();

    switch (level) {
        case 1:
            // --- Level 1 ---
            for (int i = 3; i < MAP_HEIGHT - 3; i += 3) {
                for (int j = 3; j < MAP_WIDTH - 3; j += 3) {
                    walls.emplace_back(j * TILE_SIZE, i * TILE_SIZE);
                }
            }
            enemyNumber = 3;
            break;

        case 2:
            // --- Level 2 ---
            for (int i = 2; i < MAP_HEIGHT - 2; i += 2) {
                for (int j = 2; j < MAP_WIDTH - 2; j += 2) {
                    walls.emplace_back(j * TILE_SIZE, i * TILE_SIZE);
                }
            }
            enemyNumber = 5;
            break;

        case 3:
            // --- Level 3 ---
            for (int i = 1; i < MAP_HEIGHT - 1; i++) {
                for (int j = 1; j < MAP_WIDTH - 1; j++) {
                    if ((i + j) % 4 == 0)
                        walls.emplace_back(j * TILE_SIZE, i * TILE_SIZE);
                }
            }
            enemyNumber = 7;
            break;

        default:
            // Mặc định
            for (int i = 3; i < MAP_HEIGHT - 3; i += 2) {
                for (int j = 3; j < MAP_WIDTH - 3; j += 2) {
                    walls.emplace_back(j * TILE_SIZE, i * TILE_SIZE);
                }
            }
            enemyNumber = 4;
            break;
    }

    // Spawn enemy (tương tự như spawnEnemies() trong Game)
    for (int i = 0; i < enemyNumber; i++) {
        int ex, ey;
        bool validPosition = false;
        while (!validPosition) {
            ex = (rand() % (MAP_WIDTH - 2) + 1) * TILE_SIZE;
            ey = (rand() % (MAP_HEIGHT - 2) + 1) * TILE_SIZE;
            validPosition = true;
            for (const auto& wall : walls) {
                if (wall.active && wall.x == ex && wall.y == ey) {
                    validPosition = false;
                    break;
                }
            }
        }
        enemies.emplace_back(ex, ey, renderer);
    }
}
