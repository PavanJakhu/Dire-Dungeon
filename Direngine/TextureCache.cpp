#include "TextureCache.h"
#include "Loader.h"
#include <iostream>

GLTexture TextureCache::GetTexture(std::string _texturePath) {
  // Lookup the texture and see if its in the map
  auto mit = textureMap.find(_texturePath);

  // Check if its not in the map
  if (mit == textureMap.end()) {
    //Load the texture
    GLTexture texture = Loader::LoadPNG(_texturePath);

    //Insert it into the map
    textureMap.insert(make_pair(_texturePath, texture));

    std::cout << "Loaded Texture: " << _texturePath << std::endl;
    return texture;
  }

  // If it was in the map already
  std::cout << "Used Cached Texture: " << _texturePath << std::endl;
  return mit->second;
}