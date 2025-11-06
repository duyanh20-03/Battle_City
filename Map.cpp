#include "Map.h"
#include <cstdlib>
#include <ctime>

void Map::generateLevel(int level, std::vector<Wall>& walls, std::vector<EnemyTank>& enemies, int enemyNumber, SDL_Renderer* renderer) {
    walls.clear();
    enemies.clear();
    srand(static_cast<unsigned>(time(nullptr)));

    switch (level) {
        // --- Level 1: cơ bản, dễ ---
        case 1:
            for (int i = 3; i < MAP_HEIGHT - 3; i += 3) {
                for (int j = 3; j < MAP_WIDTH - 3; j += 3) {
                    walls.emplace_back(j * TILE_SIZE, i * TILE_SIZE);
                }
            }
            enemyNumber = 3;
            break;

        // --- Level 2: dày hơn, nhiều tường hơn ---
        case 2:
            for (int i = 2; i < MAP_HEIGHT - 2; i += 2) {
                for (int j = 2; j < MAP_WIDTH - 2; j += 2) {
                    walls.emplace_back(j * TILE_SIZE, i * TILE_SIZE);
                }
            }
            enemyNumber = 5;
            break;

        // --- Level 3: hoa văn chéo, nhiều hướng tấn công ---
        case 3:
            for (int i = 1; i < MAP_HEIGHT - 1; i++) {
                for (int j = 1; j < MAP_WIDTH - 1; j++) {
                    if ((i + j) % 4 == 0)
                        walls.emplace_back(j * TILE_SIZE, i * TILE_SIZE);
                }
            }
            enemyNumber = 7;
            break;

        // --- Level 4: dạng mê cung (maze) ---
        case 4:
            for (int i = 1; i < MAP_HEIGHT - 1; i++) {
                for (int j = 1; j < MAP_WIDTH - 1; j++) {
                    if ((i % 2 == 0 && j % 4 != 0) || (j % 6 == 0 && i % 3 != 0)) {
                        walls.emplace_back(j * TILE_SIZE, i * TILE_SIZE);
                    }
                }
            }
            enemyNumber = 8;
            break;

        // --- Level 5: bố cục xen kẽ, có khu vực trống và tường ---
        case 5:
            for (int i = 1; i < MAP_HEIGHT - 1; i++) {
                for (int j = 1; j < MAP_WIDTH - 1; j++) {
                    if ((i % 5 == 0 && j % 2 == 0) || (i % 3 == 0 && j % 4 == 0)) {
                        walls.emplace_back(j * TILE_SIZE, i * TILE_SIZE);
                    }
                }
            }
            enemyNumber = 10;
            break;

        // --- Level 6: đối xứng, nhiều vật cản nhỏ (hard mode) ---
        case 6:
            for (int i = 1; i < MAP_HEIGHT - 1; i++) {
                for (int j = 1; j < MAP_WIDTH - 1; j++) {
                    if ((i % 2 == 0 && j % 2 == 0) && (i > 2 && i < MAP_HEIGHT - 3)) {
                        walls.emplace_back(j * TILE_SIZE, i * TILE_SIZE);
                    }
                    // tạo đối xứng ngang
                    if (i % 3 == 0 && j % 3 == 0 && j < MAP_WIDTH / 2) {
                        walls.emplace_back((MAP_WIDTH - j) * TILE_SIZE, i * TILE_SIZE);
                    }
                }
            }
            enemyNumber = 12;
            break;

        // --- Mặc định ---
        default:
            for (int i = 3; i < MAP_HEIGHT - 3; i += 2) {
                for (int j = 3; j < MAP_WIDTH - 3; j += 2) {
                    walls.emplace_back(j * TILE_SIZE, i * TILE_SIZE);
                }
            }
            enemyNumber = 4;
            break;
    }

    // --- Spawn enemy ---
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
