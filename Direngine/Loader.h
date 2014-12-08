#pragma once
#include "GLTexture.h"
#include "Sound.h"
#include <string>

//Loads images into GLTextures
class Loader {
public:
  static GLTexture LoadPNG(std::string _filePath);
  static Sound LoadChunk(std::string _filePath);
  static Music LoadMusic(std::string _filePath);
};