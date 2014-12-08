#pragma once
#include <Direngine/SceneManager.h>
#include <Direngine/GameWorld.h>
#include <Direngine/Sprite.h>
#include <Direngine/AudioManager.h>

class LoadingScene :
	public Scene
{
public:
  void Load();
  void Unload();
  void Update(GameTime& _gameTime);
  void Render(SpriteBatch& _spriteBatch);

  LoadingScene(SceneManager* _manager);
  void LoadTextures(int textureToLoad);
private:
  InputManager* input;
  Sprite* loadingSplash;
  Sprite* loadingSplash2;
  Music loadingMusic;  
  SceneManager* sceneManager;
  bool drawn;
  int nextTexture;
};

