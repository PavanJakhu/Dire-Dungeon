#pragma once
#include <Direngine/SceneManager.h>
#include <Direngine/GameWorld.h>
#include <Direngine/Camera2D.h>
#include <Direngine/Sprite.h>
#include <Direngine/AudioManager.h>

#include "RagdollCharacter.h"
#include "IronMaiden.h"
#include "Guillotine.h"
#include "Logs.h"
#include "Pendulum.h"
#include "BallAndChain.h"


#define SPRITENUMBER 9
#define TRAP_DELETE_DELAY 2

// Forward declaration
class Level1Scene;
class MyContactListener;

class MyContactListener : public b2ContactListener {
  Level1Scene* level;

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);  
public:
  MyContactListener(Level1Scene* _theLevel) : level(_theLevel) {}
};

class Level1Scene : public Scene {
  friend class MyContactListener;
public:
  void Load();
  void Unload();
  void Update(GameTime& _gameTime);
  void Render(SpriteBatch& _spriteBatch);  
  void UpdateCamera(float xVelocity);
  void NextLevel();
  void GameOver();

  Level1Scene(SceneManager* _manager);
private:
  InputManager* input;
  Camera2D* camera;

  GameWorld* world;
  GameTime time;
  RagdollCharacter* ragdoll;
  IronMaiden* ironMaiden;
  Guillotine* guillotine;
  Logs* logs;
  Pendulum* pendulum;
  BallAndChain* ballAndChain;
  MyContactListener* myContactListenerInstance;
  int currentUISelection;
  int currentLevel;
  int trapsHit;
  int score;
  float characterSpeed;
  Sprite *sprites[SPRITENUMBER]; // array of non-collidable sprites  

  Music levelMusic;
  Music winMusic;
};