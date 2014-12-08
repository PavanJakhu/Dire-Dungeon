#include "AudioManager.h"
#include "Errors.h"

bool AudioManager::IsPlayingMusic() {
  return Mix_PlayingMusic();
}

void AudioManager::PlayMusic(Music _music, bool _loop) {
  if (IsPlayingMusic())
    StopPlayingMusic();

  if (Mix_PlayMusic(_music.music, _loop) == -1)
    Debug::Log("Music failed to play: " + std::string(Mix_GetError()));
}

void AudioManager::PlaySound(Sound _sound, int _numloops) {
  Mix_PlayChannel(-1, _sound.sound, _numloops);
}

void AudioManager::StopPlayingMusic() {
  Mix_HaltMusic();
}

void AudioManager::PauseToggleMusic() {
  if (Mix_PausedMusic())
    Mix_ResumeMusic();
  else
    Mix_PauseMusic();
}