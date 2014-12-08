#include "Window.h"
#include "Errors.h"

void Window::Create(std::string _windowName, int _screenWidth, int _screenHeight, unsigned int _currentFlags) {
  Uint32 flags = SDL_WINDOW_OPENGL;
  screenWidth = _screenWidth;
  screenHeight = _screenHeight;

  if (_currentFlags & INVISIBLE)
    flags |= SDL_WINDOW_HIDDEN;
  if (_currentFlags & FULLSCREEN)
    flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
  if (_currentFlags & BORDERLESS)
    flags |= SDL_WINDOW_BORDERLESS;

  //Open an SDL window
  sdlWindow = SDL_CreateWindow(_windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, flags);
  if (sdlWindow == nullptr)
    Debug::FatalError("SDL Window could not be created!");

  //Set up our OpenGL context
  SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindow);
  if (glContext == nullptr)
    Debug::FatalError("SDL_GL context could not be created!");

  //Set up glew (optional but recommended)
  GLenum error = glewInit();
  if (error != GLEW_OK)
    Debug::FatalError("Could not initialize glew!");

  //Check the OpenGL version
  std::printf("***   OpenGL Version: %s   ***\n", glGetString(GL_VERSION));

  //Set the background color to blue
  glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

  //Set VSYNC
  SDL_GL_SetSwapInterval(0);

  // Enable alpha blend
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::SwapBuffer() {
  SDL_GL_SwapWindow(sdlWindow);
}