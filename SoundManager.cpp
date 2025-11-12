#include "SoundManager.h"

SoundManager::SoundManager()
       : explodeSound(nullptr),
        shootSound(nullptr),
        backgroundSound(nullptr),
        winSound(nullptr){}

SoundManager::~SoundManager() {
    cleanup();
}

void SoundManager::init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
    }

    explodeSound = Mix_LoadWAV("explodeSound.wav");
    shootSound = Mix_LoadWAV("shootSound.wav");
    backgroundSound = Mix_LoadWAV("bgSound.wav");
    winSound = Mix_LoadWAV("winSound.wav");
}

void SoundManager::playExplosion() {
    if (explodeSound) Mix_PlayChannel(-1, explodeSound, 0);
}

void SoundManager::playShoot() {
    if (shootSound) Mix_PlayChannel(-1, shootSound, 0);
}

void SoundManager::playBackground() {
    if (backgroundSound) Mix_PlayChannel(-1, backgroundSound, 0);
}

void SoundManager::playWinSound() {
    if (winSound) Mix_PlayChannel(-1, winSound, 0);
}

void SoundManager::cleanup() {
    if (explodeSound) Mix_FreeChunk(explodeSound);
    if (shootSound) Mix_FreeChunk(shootSound);
    if (backgroundSound) Mix_FreeChunk(backgroundSound);
    if (winSound) Mix_FreeChunk(winSound);

    Mix_CloseAudio();
}
