#pragma once
#include <Direngine/SceneManager.h>
#include <Direngine/Sprite.h>

class CreditScene :
	public Scene
{
public:
  void Load();
  void Unload();
  void Update(GameTime& _gameTime);
  void Render(SpriteBatch& _spriteBatch);

  CreditScene(SceneManager* _manager);

private:
  InputManager* input;
  
  // GameWorld* world;
  //Ball* ball;
  //TestObject* test;
  Sprite* creditSplash;
};

