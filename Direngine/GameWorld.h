#pragma once
#include <Box2D/Box2D.h>
#include <vector>
#include "GLTexture.h"
#include "SpriteBatch.h"
#include "GameTime.h"

// Forward declaration
class GameWorld;
class GameObject;

class GameWorld {
public:
	// Add an object to the Box2D world
	void AddObject(GameObject* _gameObject);
	// Remove and delete objects from the Box2D world
	void DestroyObject(GameObject* _gameObject);

	void Update(GameTime& _gameTime);
	void Draw(SpriteBatch& _spriteBatch);

	b2World* GetB2World() { return world; }

	GameWorld(b2Vec2 _gravity);
	~GameWorld();
private:
	b2World* world;
	std::vector<GameObject*> objects;
};

class GameObject {
public:
	// For non-Box2D related updating
	virtual void Update(GameTime& _gameTime) {}
	// Drawing the bodies to the spritebatch
	virtual void Draw(SpriteBatch& _spriteBatch) {}

	// Get a body from this object (default 0, the primary body)
	b2Body* GetBody(int _body = 0);
	b2Joint* GetJoint(int _joint = 0);
	int GetBodyCount();
	int GetJointCount();

	// Remove the object from the world
	void Destroy();

	GameObject(GameWorld* _world) : world(_world) {}
	// Remove any bodies / joints from the world
	virtual ~GameObject();
protected:
	// The world which this object belongs to
	GameWorld* world;
	// The bodies that belong to this object
	std::vector<b2Body*> bodies;
	// The joints that belong to this object
	std::vector<b2Joint*> joints;
};