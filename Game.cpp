#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Game.h"
#include "Constant.h"
#include "Map.h"

Game::Game() {
    // --- Khởi tạo SDL ---
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        running = false;
        return;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init failed: " << IMG_GetError() << "\n";
        running = false;
        return;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << "\n";
        running = false;
        return;
    }

    // --- Tạo cửa sổ ---
    window = SDL_CreateWindow(
        "Battle City",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        running = false;
        return;
    }

    // --- Renderer ---
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        running = false;
        return;
    }

    // --- Load hình tim (mạng sống) ---
    const char* heartFiles[3] = {"1-Lives.png", "2-Lives.png", "3-Lives.png"};
    for (int i = 0; i < 3; i++) {
        SDL_Surface* tempSurface = IMG_Load(heartFiles[i]);
        if (tempSurface) {
            hearts[i] = SDL_CreateTextureFromSurface(renderer, tempSurface);
            SDL_FreeSurface(tempSurface);
        } else {
            std::cerr << "Failed to load " << heartFiles[i]
                      << ": " << IMG_GetError() << std::endl;
            hearts[i] = nullptr;
        }
    }

    // --- Khởi tạo biến ---
    running = true;
    player.init(renderer);
    soundManager.init();
    explodeTexture = IMG_LoadTexture(renderer, "explode.png");
}

void Game::render() {
    // --- Clear background ---
    SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // --- Vẽ nền bản đồ ---
    for (int i = 1; i < MAP_HEIGHT - 1; i++) {
        for (int j = 1; j < MAP_WIDTH - 1; j++) {
            SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            SDL_RenderFillRect(renderer, &tile);
        }
    }

    // --- Vẽ tường ---
    for (auto& wall : walls)
        if (wall.active) wall.render(renderer);

    // --- Vẽ player, enemy, explosion ---
    player.render(renderer);
    for (auto& enemy : enemies) enemy.render(renderer);
    for (auto& exp : explosions)
        if (exp.active) exp.render(renderer);

    // --- Hiển thị mạng ---
    if (lives > 0 && lives <= 3) {
        SDL_Texture* currentHeart = hearts[lives - 1];
        if (currentHeart) {
            SDL_Rect heartRect = { SCREEN_WIDTH - 110, 10, 100, 30 };
            SDL_RenderCopy(renderer, currentHeart, nullptr, &heartRect);
        }
    }

    SDL_RenderPresent(renderer);
}

void Game::handleEvents(float deltaTime) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            std::cout << "Pressed: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
            if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                std::cout << "ENTER DETECTED!\n";
                player.shoot();
                soundManager.playShoot();
            }
        }
    }

    // --- Xử lý phím di chuyển ---
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    float speed = 200.0f;

    if (keystate[SDL_SCANCODE_UP]) {
        player.move(0, -speed, deltaTime, walls);
        player.dirX = 0; player.dirY = -1;
    }
    if (keystate[SDL_SCANCODE_DOWN]) {
        player.move(0, speed, deltaTime, walls);
        player.dirX = 0; player.dirY = 1;
    }
    if (keystate[SDL_SCANCODE_RIGHT]) {
        player.move(speed, 0, deltaTime, walls);
        player.dirX = 1; player.dirY = 0;
    }
    if (keystate[SDL_SCANCODE_LEFT]) {
        player.move(-speed, 0, deltaTime, walls);
        player.dirX = -1; player.dirY = 0;
    }

    //Pause;
    if (keystate[SDL_SCANCODE_ESCAPE]) {
        pauseScreen ps(renderer);
        ps.init();
         bool isPause = true;
         SDL_Event e;
         while (isPause) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false;
                    return;
                }
                ps.handleEvent(&e);
            }

            std::string click = ps.getClick();
            if (click == "Resume") {
                isPause = false;
            }
            else if (click == "Menu") {
                running = false;
                return;
            }
            else if (click == "Quit") {
                running = false;
                exit(0);
            }
            ps.render();

         }
         ps.cleanup();
    }
}

void Game::update(float deltaTime) {
    // --- Cập nhật đạn player ---
    player.updateBullets();
    for (auto& bullet : player.bullets) {
        for (auto& wall : walls) {
            if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                wall.active = false;
                bullet.active = false;
                explosions.push_back(Explode(wall.x, wall.y, explodeTexture));
                soundManager.playExplosion();
                break;
            }
        }
    }

    // --- Enemy update ---
    for (auto& enemy : enemies) {
        enemy.move(deltaTime, walls);
        enemy.updateBullet();
        if (rand() % 100 < 2) enemy.shoot();
    }

    // --- Enemy bullet vs wall ---
    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.bullets) {
            for (auto& wall : walls) {
                if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                    wall.active = false;
                    bullet.active = false;
                    explosions.push_back(Explode(wall.x, wall.y, explodeTexture));
                    soundManager.playExplosion();
                    break;
                }
            }
        }
    }

    // --- Player bullet vs enemy ---
    for (auto& bullet : player.bullets) {
        for (auto& enemy : enemies) {
            if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                enemy.active = false;
                bullet.active = false;
                explosions.push_back(Explode(enemy.x, enemy.y, explodeTexture));
                soundManager.playExplosion();
                break;
            }
        }
    }

    // --- Explosion update ---
    for (auto& exp : explosions) exp.update(deltaTime);

    // --- Xóa enemy chết ---
    enemies.erase(remove_if(enemies.begin(), enemies.end(),
                            [](EnemyTank& e) { return !e.active; }),
                  enemies.end());

    // --- Enemy bullet vs player ---
    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.bullets) {
            if (SDL_HasIntersection(&bullet.rect, &player.rect)) {
                bullet.active = false;
                lives--;
                explosions.push_back(Explode(player.x, player.y, explodeTexture));
                soundManager.playExplosion();

                if (lives > 0) {
                    player = Player(TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer);
                } else {
                    running = false;
                }
                break;
            }
        }
    }

    // --- Xóa explosion đã hết ---
    explosions.erase(remove_if(explosions.begin(), explosions.end(),
                               [](Explode& e) { return !e.active; }),
                     explosions.end());
}

void Game::run() {
    while (true) {
        soundManager.playBackground();

        MenuScreen menu(renderer);
        menu.init();
        bool isMenu = true;
        SDL_Event e;

        // --- Menu loop ---
        while (isMenu) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false;
                    return;
                }
                menu.handleEvent(&e);
            }

            std::string click = menu.getClick();
            if (click == "Play") {
                currentLevel = 1;
                isMenu = false;
                running = true;
            } else if (click == "Levels") {
                menu.initLevels();
            } else if (click.rfind("Level ", 0) == 0) {
                currentLevel = std::stoi(click.substr(6));
                std::cout << "Starting " << click << "!\n";
                isMenu = false;
                running = true;
            } else if (click == "Back") {
                menu.SetInLevelSelect(false);
            } else if (click == "Quit") {
                running = false;
                return;
            }

            menu.render();
            SDL_Delay(16);
        }

        menu.cleanup();

        // --- Load map ---
        Map::generateLevel(currentLevel, walls, enemies, enemyNumber, renderer);
        player = Player(TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer);

        const int frameDelay = 16;
        Uint32 lastTime = SDL_GetTicks();

        Win winScreen(renderer);
        bool levelCompleted = false;

        // --- Game loop ---
        while (running && !levelCompleted) {
            Uint32 currentTime = SDL_GetTicks();
            float deltaTime = (currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;

            handleEvents(deltaTime);
            update(deltaTime);
            render();

            if (enemies.empty()) {
                levelCompleted = true;
            }

            Uint32 frameTime = SDL_GetTicks() - currentTime;
            if (frameTime < frameDelay)
                SDL_Delay(frameDelay - frameTime);
        }

        // --- Win Screen ---
        if (levelCompleted) {
            SDL_Delay(1500);
            winScreen.init();
            soundManager.playWinSound();
            std::cout << "WinSound\n";

            bool showWin = true;
            SDL_Event e;

            while (showWin) {
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) {
                        showWin = false;
                        return;
                    }
                    winScreen.handleEvent(&e);
                }

                std::string click = winScreen.getClick();
                if (click == "Menu") {
                    showWin = false;
                } else if (click == "Levels") {
                    showWin = false;
                    menu.init();
                    menu.initLevels();
                } else if (click == "Quit") {
                    showWin = false;
                    running = false;
                    return;
                }

                winScreen.render();
                SDL_Delay(16);
            }

            winScreen.cleanup();
        }
    }
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
