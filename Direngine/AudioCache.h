#pragma once
#include "Sound.h"
#include <map>
#include <iostream>
#include "Loader.h"

// Cache for sound files
class AudioCache {
public:
  AudioCache() {}
  ~AudioCache() {}

  template <class AudioType>
  AudioType GetAudio(std::string _filePath) {
    AudioType T;
    return T;
  }

template <>
Sound AudioCache::GetAudio<Sound>(std::string _soundPath) {
  auto mit = soundMap.find(_soundPath);

  if (mit == soundMap.end()) {
    Sound sound = Loader::LoadChunk(_soundPath);

    soundMap.insert(make_pair(_soundPath, sound));

    std::cout << "Loaded Sound: " << _soundPath << std::endl;
    return sound;
  }

  // If it was in the map already
  std::cout << "Used Cached Sound: " << _soundPath << std::endl;
  return mit->second;
}

template <>
Music AudioCache::GetAudio<Music>(std::string _musicPath) {
  auto mit = musicMap.find(_musicPath);

  if (mit == musicMap.end()) {
    Music music = Loader::LoadMusic(_musicPath);

    musicMap.insert(make_pair(_musicPath, music));

    std::cout << "Loaded Music: " << _musicPath << std::endl;
    return music;
  }

  // If it was in the map already
  std::cout << "Used Cached Music: " << _musicPath << std::endl;
  return mit->second;
}
private:
  std::map<std::string, Sound> soundMap;
  std::map<std::string, Music> musicMap;
};