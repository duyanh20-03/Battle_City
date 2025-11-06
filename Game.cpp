#include<SDL.h>
#include<SDL_ttf.h>
#include "Game.h"
#include "Constant.h"
#include "Map.h"

Game::Game() {
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

    window = SDL_CreateWindow("Battle City",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        running = false;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        running = false;
        return;
    }

    const char* heartFiles[3] = {"1-Lives.png", "2-Lives.png", "3-Lives.png"};
    for (int i = 0; i < 3; i++) {
         SDL_Surface* tempSurface = IMG_Load(heartFiles[i]);
         if (tempSurface) {
             hearts[i] = SDL_CreateTextureFromSurface(renderer, tempSurface);
             SDL_FreeSurface(tempSurface);
         } else {
            std::cerr << "Failed to load " << heartFiles[i] << ": " << IMG_GetError() << std::endl;
            hearts[i] = nullptr;
    }
}


    running = true;
    player.init(renderer);
    //generateWalls();
    //spawnEnemies();
    //player = Player( TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE,renderer);

}

void Game::generateWalls() {
    for (int i = 3; i < MAP_HEIGHT - 3; i += 2) {
        for (int j = 3; j < MAP_WIDTH - 3; j += 2) {
            Wall w = Wall(j * TILE_SIZE, i * TILE_SIZE);
            walls.push_back(w);
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 125,125,125,255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer,0,0,0,255);

    for(int i=1;i<MAP_HEIGHT-1;i++){
                for(int j=1;j<MAP_WIDTH-1;j++){
                    SDL_Rect tile = {j*TILE_SIZE,i*TILE_SIZE,TILE_SIZE,TILE_SIZE};
                    SDL_RenderFillRect(renderer,&tile);
                }
            }
    for (auto& wall : walls) {
        wall.render(renderer);
    }

    player.render(renderer);
    for (auto& enemy : enemies) {
        enemy.render(renderer);
    }

     if (lives > 0 && lives <= 3) {
         SDL_Texture* currentHeart = hearts[lives - 1]; // 3 -> hearts[2], 2 -> hearts[1]
         if (currentHeart) {
             SDL_Rect heartRect = { SCREEN_WIDTH - 110, 10, 100, 30 };
             SDL_RenderCopy(renderer, currentHeart, nullptr, &heartRect);
         }
}




    SDL_RenderPresent(renderer);


}

void Game::handleEvents(float deltaTime) {
    SDL_Event event;
    double angle = 0.0;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        else if (event.type == SDL_KEYDOWN) {
            std::cout << "Pressed: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
            if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                    std::cout << "ENTER DETECTED!\n";
                player.shoot();
            }
        }
    }

            const Uint8* keystate = SDL_GetKeyboardState(NULL);
            float speed = 200.0f;
            if (keystate[SDL_SCANCODE_UP]) {
                player.move(0, -speed, deltaTime, walls);
                player.dirX = 0;
                player.dirY = -1;
            }
            if (keystate[SDL_SCANCODE_DOWN]) {
                player.move(0, speed, deltaTime, walls);
                player.dirX = 0;
                player.dirY = 1;
            }
            if (keystate[SDL_SCANCODE_RIGHT]) {
                player.move(speed, 0, deltaTime, walls);
                player.dirX = 1;
                player.dirY = 0;
            }
            if (keystate[SDL_SCANCODE_LEFT]) {
                player.move(-speed, 0, deltaTime, walls);
                player.dirX = -1;
                player.dirY = 0;
            }

        }


void Game::update(float deltaTime) {
    player.updateBullets();
    for (auto& bullet : player.bullets) {
        for (auto& wall : walls) {
            if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                wall.active = false;
                bullet.active = false;
                break;
            }
        }
    }
    for(auto& enemy : enemies){
        enemy.move(deltaTime, walls);
        enemy.updateBullet();
        if(rand() % 100 <2){
            enemy.shoot();
        }
    }
    for(auto& enemy : enemies){
        for(auto& bullet : enemy.bullets){
            for(auto& wall : walls){
                if(wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)){
                    wall.active = false;
                    bullet.active = false;
                    break;
                }
            }
        }
    }
    for(auto& bullet : player.bullets){
            for(auto& enemy : enemies){
                if(enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)){
                    enemy.active = false;
                    bullet.active = false;
                    break;
            }
        }
    }
    enemies.erase(remove_if(enemies.begin(),enemies.end(),[](EnemyTank &e) {return !e.active; }), enemies.end());


    for(auto& enemy : enemies){
        for(auto& bullet : enemy.bullets){
            if(SDL_HasIntersection(&bullet.rect,&player.rect)){
                bullet.active = false;
                lives--;
                if (lives > 0) {
                    player = Player(TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer);
                }
                else {
                    running = false;

                }
                break;
            }
        }
    }
}

void Game::spawnEnemies() {
    enemies.clear();
    for (int i = 0; i < enemyNumber; i++) {
        int ex,ey;
        bool validPosition=false;
        while(!validPosition){
            ex= (rand() % (MAP_WIDTH-2) +1)*TILE_SIZE;
            ey=(rand() % (MAP_HEIGHT-2) +1)*TILE_SIZE;
            validPosition=true;
            for(const auto& wall:walls){
                if(wall.active && wall.x==ex && wall.y==ey){
                    validPosition=false;
                    break;
                }
            }
        }
        enemies.push_back(EnemyTank(ex, ey, renderer));
    }
}

void Game::run() {
   while (true) {
    MenuScreen menu(renderer);

    menu.init();
    bool isMenu = true;
    SDL_Event e;

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
        }
        else if (click == "Levels") {
            menu.initLevels();
        }
        else if (click == "Level 1") {
            std::cout << "Starting Level 1!\n";
            isMenu = false; // load map level 1
            running = true;
        }
        else if (click == "Level 2") {
            std::cout << "Starting Level 2!\n";
            currentLevel = 2;
            isMenu = false;
            running = true;
        }
        else if (click == "Level 3") {
            std::cout << "Starting Level 3!\n";
            currentLevel = 3;
            isMenu = false;
            running = true;
        }
        else if (click == "Level 4") {
            std::cout << "Starting Level 4!\n";
            currentLevel = 4;
            isMenu = false;
            running = true;
        }
        else if (click == "Level 5") {
            std::cout << "Starting Level 5!\n";
            currentLevel = 5;
            isMenu = false;
            running = true;
        }
        else if (click == "Level 6") {
            std::cout << "Starting Level 6!\n";
            currentLevel = 6;
            isMenu = false;
            running = true;
        }
        else if (click == "Back") {
            menu.SetInLevelSelect(false);
        }
        else if (click == "Quit") {
            running = false;
            return;
        }
        menu.render();
        SDL_Delay(16);
    }
    menu.cleanup();
    Map::generateLevel(currentLevel, walls, enemies, enemyNumber, renderer);
    player = Player( TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE,renderer);

    const int frameDelay = 16; // ~60 FPS
    Uint32 lastTime = SDL_GetTicks();

    Win winScreen(renderer);       // thêm: màn hình Win
    bool levelCompleted = false;
    while (running && !levelCompleted) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f; // giây
        lastTime = currentTime;

        handleEvents(deltaTime);
        update(deltaTime);
        render();
        if (enemies.empty()) {
            levelCompleted = true;  // đánh dấu level hoàn thành
            running = false;        // dừng vòng loop main level
        }
        Uint32 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < frameDelay) SDL_Delay(frameDelay - frameTime);
    }

    if (levelCompleted) {
    winScreen.init();           // load hình và nút
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
            // trở về menu
        }
        else if (click == "Levels") {
            showWin = false;
            menu.initLevels();

            // mở màn hình chọn level
        }
        else if (click == "Quit") {
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
