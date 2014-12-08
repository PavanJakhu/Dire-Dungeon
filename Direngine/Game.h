#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <SDL/SDL_mixer.h>
#include <string>
#include "Window.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "AudioManager.h"
#include "GameTime.h"

enum class GameState { PLAY, EXIT };

class Game {
public:
  void Run();
  void Exit() { state = GameState::EXIT; }
protected:
  Window window;
  std::string gameTitle;
  int screenWidth, screenHeight;
  float maxFPS;
  GameState state;

  InputManager* input;
  ResourceManager* resources;

  virtual void Init() = 0;
  virtual void Load() = 0;
  virtual void Unload() = 0;
  virtual void Update(GameTime& _gameTime) = 0;
  virtual void Render() = 0;

  void SetTitle(std::string _gameTitle) { gameTitle = _gameTitle; }
  void SetDimensions(int _screenWidth, int _screenHeight) { screenWidth = _screenWidth; screenHeight = _screenHeight; }
  void SetFPSLimit(float _maxFPS) { maxFPS = _maxFPS; }
private:
  void Quit();
};