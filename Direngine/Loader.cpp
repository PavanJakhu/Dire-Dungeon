#include "Loader.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "Errors.h"

GLTexture Loader::LoadPNG(std::string _filePath) {
  // Create a GLTexture and initialize all its fields to 0
  GLTexture texture = {};

  // This is the input data to decodePNG, which we load from a file
  std::vector<unsigned char> in;
  // This is the output data from decodePNG, which is the pixel data for our texture
  std::vector<unsigned char> out;

  unsigned long width, height;

  // Read in the image file contents into a buffer
  if (IOManager::ReadFileToBuffer(_filePath, in) == false)
    Debug::FatalError("Failed to load PNG file to buffer!");

  // Decode the .png format into an array of pixels
  int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
  if (errorCode != 0)
    Debug::FatalError("decodePNG failed with error: " + std::to_string(errorCode));

  // Generate the OpenGL texture object
  glGenTextures(1, &(texture.id));

  // Bind the texture object
  glBindTexture(GL_TEXTURE_2D, texture.id);
  // Upload the pixels to the texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

  // Set some texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  // Generate the mipmaps
  glGenerateMipmap(GL_TEXTURE_2D);

  // Unbind the texture
  glBindTexture(GL_TEXTURE_2D, 0);

  texture.width = width;
  texture.height = height;

  // Return a copy of the texture data
  return texture;
}

Sound Loader::LoadChunk(std::string _filePath) {
  Sound sound;
  sound.sound = nullptr;

  // Make chunk a reference pointer to sound (changes to it change sound)
  Mix_Chunk*& chunk = sound.sound;
  chunk = Mix_LoadWAV(_filePath.c_str());
  if (chunk == nullptr)
    Debug::FatalError("Failed to load sound: " + _filePath + Mix_GetError());

  return sound;
}

Music Loader::LoadMusic(std::string _filePath) {
  Music music;
  music.music = nullptr;

  // Make music a reference pointer to music
  Mix_Music*& chunk = music.music;
  chunk = Mix_LoadMUS(_filePath.c_str());
  if (chunk == nullptr)
    Debug::FatalError("Failed to load music: " + _filePath + Mix_GetError());

  return music;
}