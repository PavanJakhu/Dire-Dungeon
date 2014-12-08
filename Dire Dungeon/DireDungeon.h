#pragma once
#include <Direngine/Game.h>
#include <Direngine/GLSLProgram.h>
#include <Direngine/SpriteBatch.h>
#include <Direngine/SceneManager.h>
#include <Direngine/Camera2D.h>
#include <Direngine/Sprite.h>

class DireDungeon : public Game {
private:
  GLSLProgram shader;
  Camera2D camera;
  SpriteBatch spriteBatch;
  SceneManager* sceneManager;
  float bgColor;

	Sprite* test;
protected:
  void Init();
  void Load();
  void Unload();
  void Update(GameTime& _gameTime);
  void Render();
public:
  Camera2D* GetCamera() { return &camera; }
};