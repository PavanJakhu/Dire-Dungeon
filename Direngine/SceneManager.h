#pragma once
#include "Game.h"
#include "SpriteBatch.h"
#include <unordered_map>

// Forward declaration
class Scene;
class SceneManager;

class Scene {
public:
  virtual void Load() = 0;
  virtual void Unload() = 0;
  virtual void Update(GameTime& _gameTime) = 0;
  virtual void Render(SpriteBatch& _spriteBatch) = 0;

  Scene(SceneManager* _manager) : manager(_manager) {}
  virtual ~Scene() {}
protected:
  SceneManager* manager;
};

class SceneManager {
private:
  std::unordered_map<std::string, Scene*> scenes;
  Scene* currentScene;
  Game* theGame;
public:
  void Update(GameTime& _gameTime);
  void Render(SpriteBatch& _spriteBatch);

  void AddScene(std::string _name, Scene* _scene);
  void ChangeScene(std::string _sceneName);

  Game* TheGame() { return theGame; }

  SceneManager(Game* _game);
  ~SceneManager();
};