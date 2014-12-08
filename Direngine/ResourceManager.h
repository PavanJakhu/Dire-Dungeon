#pragma once
#include "TextureCache.h"
#include "AudioCache.h"
#include <string>

// The class that handles loading and storing of texture data
class ResourceManager {
public:
  // Get (and create) the ResourceManager instance
  static ResourceManager* GetInstance();
  // Deletes the resource manager and the pointer to it
  void DestroyResourceManager(ResourceManager*&);

  // Default template that gets triggered if an invalid template argument is used
  template <class T> T Load(std::string _path) {
    T myT;
    Debug::FatalError("Invalid resource type.");
    return myT; // What a fun workaround that shouldn't compile
  }
  // Load a GLTexture
  template<> GLTexture Load<GLTexture>(std::string _path) {
    return textureCache.GetTexture(_path);
  }
  // Load a Sound
  template <> Sound Load<Sound>(std::string _path) {
    return audioCache.GetAudio<Sound>(_path);
  }
  // Load a Music
  template <> Music Load<Music>(std::string _path) {
    return audioCache.GetAudio<Music>(_path);
  }
private:
  TextureCache textureCache;
  AudioCache audioCache;

  ResourceManager() {}
  ~ResourceManager() {}
};