#pragma once
#include <Direngine/SceneManager.h>
#include <Direngine/GameWorld.h>
#include "Torch.h"
#include <Direngine/AudioManager.h>


class TitleScene :
	public Scene
{
public:
  void Load();
  void Unload();
  void Update(GameTime& _gameTime);
  void Render(SpriteBatch& _spriteBatch);

  TitleScene(SceneManager* _manager);

private:
  InputManager* input;
  Sound doorSlam;
  Music titleMusic;
  Sprite* titleSplash;
  Torch* torch;
  int currentCursorPosition;

};

