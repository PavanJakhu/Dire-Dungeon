#pragma once
#include <Direngine/SceneManager.h>
#include <Direngine/GameWorld.h>
#include <Direngine/Camera2D.h>
#include <Direngine/Sprite.h>

#include "Box.h"
#include "Ball.h"
#include "Ground.h"
<<<<<<< HEAD
#include "RagdollCharacter.h"
=======
#include "TestObject.h"
#include "Wall.h"

#define SPRITENUMBER 7
>>>>>>> origin/master

class Level1Scene : public Scene {
public:
  void Load();
  void Unload();
  void Update(GameTime& _gameTime);
  void Render(SpriteBatch& _spriteBatch);
  void UpdateCamera(float xVelocity);

  Level1Scene(SceneManager* _manager);
private:
  InputManager* input;
  Camera2D* camera;

  GameWorld* world;
<<<<<<< HEAD
  GameObject *t1, *t2;
};

class MyContactListener : public b2ContactListener{
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
=======
  Ball* ball;
  TestObject* test;

  Sprite *sprites[SPRITENUMBER];
  
  
>>>>>>> origin/master
};