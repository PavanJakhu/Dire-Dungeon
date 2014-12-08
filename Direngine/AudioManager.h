#pragma once
#include "Sound.h"

// Class that handles global audio
class AudioManager {
public:
  // Stop the previous music from playing and play new music
  static void PlayMusic(Music _music, bool _loop);
  // Play a sound
  static void PlaySound(Sound _sound, int _numloops = 0);
  // Returns whether or not music is playing
  static bool IsPlayingMusic();
  // Stops the currently playing music
  static void StopPlayingMusic();
  // Toggle pause state
  static void PauseToggleMusic();
};