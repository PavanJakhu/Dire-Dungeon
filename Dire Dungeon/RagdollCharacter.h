#pragma once
#include <Direngine/GameWorld.h>
#include <Direngine/ResourceManager.h>
#include <Direngine/AudioManager.h>

class RagdollCharacter : public GameObject {
public:
	RagdollCharacter(float _scale, float _speed, b2Vec2 _position, GameWorld* _world, ResourceManager* _resources);
	~RagdollCharacter();
	void Update(GameTime& _gameTime);
	void Draw(SpriteBatch& _spriteBatch);	
	float playerVelocity();
	b2Vec2 playerPosition();
	void startContact();
	void endContact();
	void moveCharacter(float speed);
	void FlipSpeed();
	float speed;	
	
private:
	GLTexture boxTexture;
	GLTexture headHappyTexture;
	GLTexture headSadTexture;
	GLTexture headTexture;
	float startTime;
	float isContactRagdoll;
	bool audioPlayed;
	Sound onHitScream;
	bool recentCollision;
	void CreateBody(int bodyCount, float x, float y, float w, float h, float angle = 0);
	void CreateJoint(b2Body* bodyA, b2Body* bodyB, float anchorAX, float anchorAY, float anchorBX, float anchorBY, bool limited, float lowerAngle = 0, float upperAngle = 0);
	std::vector<b2Vec2> bodyInitialPos;
};
