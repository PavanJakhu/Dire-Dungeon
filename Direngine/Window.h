#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

enum WindowFlags {
  INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4
};

class Window {
public:
  void Create(std::string _windowName, int _screenWidth, int _screenHeight, unsigned int _flags);
  void SwapBuffer();

  int GetWidth() { return screenWidth; }
  int GetHeight() { return screenHeight; }

  Window() {}
  ~Window() {
    SDL_DestroyWindow(sdlWindow);
  }
private:
  SDL_Window* sdlWindow;
  int screenWidth, screenHeight;
};