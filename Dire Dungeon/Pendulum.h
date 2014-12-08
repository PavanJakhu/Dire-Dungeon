#pragma once
#include <Direngine\GameWorld.h>
class Pendulum : public GameObject
{
public:
	Pendulum(b2Vec2 _position, GameWorld* _world, GLTexture _pendulumTexture);
	~Pendulum(void);

	void Update(GameTime& _gameTime);
	void Draw(SpriteBatch& _spriteBatch);
	b2Vec2 ApplyForce(GameTime _time);
	void Trigger();
	float triggerTime;
	bool triggered;

private:
	GLTexture pendulumTexture;
	GLTexture anchorTexture;

	b2Vec2 previousVelocity;

	const float BLADE_WIDTH, BLADE_HEIGHT;
	const float ROD_WIDTH, ROD_HEIGHT;

};

