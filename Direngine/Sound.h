#pragma once
#include <SDL/SDL_mixer.h>

struct Sound {
  Mix_Chunk* sound;
};

struct Music {
  Mix_Music* music;
};