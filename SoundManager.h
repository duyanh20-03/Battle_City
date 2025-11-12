#ifndef SOUNDMANAGER_H_INCLUDED
#define SOUNDMANAGER_H_INCLUDED

#include<SDL_mixer.h>
#include<iostream>
#include<string>

class SoundManager {
private:
    Mix_Chunk* explodeSound;
    Mix_Chunk* shootSound;
    Mix_Chunk* backgroundSound;
    Mix_Chunk* winSound;

public:
    SoundManager();
    ~SoundManager();
    void init();
    void playExplosion();
    void playShoot();
    void playBackground();
    void playWinSound();
    void cleanup();
};

#endif // SOUNDMANAGER_H_INCLUDED
