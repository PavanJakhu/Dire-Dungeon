#include "SceneManager.h"
#include "Errors.h"

SceneManager::SceneManager(Game* _game) {
  theGame = _game;
  currentScene = nullptr;
}

SceneManager::~SceneManager() {
  for (auto it = scenes.begin(); it != scenes.end(); it++) {
    it->second->Unload(); // Unload the scene
    delete it->second; // Delete the Scene*
  }
}

void SceneManager::AddScene(std::string _name, Scene* _scene) {
  scenes.insert(make_pair(_name, _scene));
}

void SceneManager::ChangeScene(std::string _sceneName) {
  if (scenes.find(_sceneName) == scenes.end()) {
    Debug::Log("Scene doesn't exist: " + _sceneName);
    return;
  }

  if (currentScene) {
    currentScene->Unload();
    currentScene = nullptr;
  }

  currentScene = scenes[_sceneName];
  currentScene->Load();
}

void SceneManager::Update(GameTime& _gameTime) {
  currentScene->Update(_gameTime);
}

void SceneManager::Render(SpriteBatch& _spriteBatch) {
  currentScene->Render(_spriteBatch);
}