#pragma once
#include <Direngine\GameWorld.h>
class BallAndChain : public GameObject
{
public:
	BallAndChain(b2Vec2 _position, GameWorld* _world, GLTexture _ballTexture, GLTexture _chainTexture);
	~BallAndChain(void);

	void Update(GameTime& _gameTime);
	void Draw(SpriteBatch& _spriteBatch);
	void Trigger();
	float triggerTime;
	bool triggered;
	b2Vec2 ApplyForce(GameTime _time);


private:
	GLTexture ballTexture;
	GLTexture chainTexture;

	const float BALL_RADIUS;
	const float CHAIN_WIDTH, CHAIN_HEIGHT;

	b2Vec2 previousVelocity;
};

