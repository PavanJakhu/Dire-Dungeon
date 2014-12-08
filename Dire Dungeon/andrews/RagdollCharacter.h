#pragma once
#include <Direngine/GameWorld.h>
#include <Direngine/ResourceManager.h>

class RagdollCharacter : public GameObject {
public:
	RagdollCharacter(b2Vec2 _position, GameWorld* _world, ResourceManager* _resources);

	void Update(GameTime& _gameTime);
	void Draw(SpriteBatch& _spriteBatch);

	float playerVelocity();
	void beginContact();
	void endContact();
private:
	GLTexture boxTexture;
	GLTexture headHappyTexture;
	GLTexture headSadTexture;
	GLTexture headTexture;
	bool contacting;
	float startTime;

	void CreateBody(int bodyCount, float x, float y, float w, float h, float angle = 0);
	void CreateJoint(b2Body* bodyA, b2Body* bodyB, float anchorAX, float anchorAY, float anchorBX, float anchorBY, bool limited, float lowerAngle = 0, float upperAngle = 0);
};