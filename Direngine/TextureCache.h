#pragma once
#include "GLTexture.h"
#include <map>

// This caches the textures so that multiple sprites can use the same textures
class TextureCache {
public:
  TextureCache() {}
  ~TextureCache() {}

  GLTexture GetTexture(std::string _texturePath);
private:
  // A map of textures, keyed by their filepath
  std::map<std::string, GLTexture> textureMap;
};