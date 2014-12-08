#pragma once
#include <Direngine/GameWorld.h>
#include <Direngine/Sprite.h>

const int SPRITES_IN_CURSOR = 3;

class Torch
{
public:
	Torch(b2Vec2 _position);
	~Torch(void);
	void Update(GameTime& _gameTime);
	void Render(SpriteBatch& _spriteBatch);

	void Move(b2Vec2 velocity);
	b2Vec2 GetPosition();
private:
	Sprite*	torch[SPRITES_IN_CURSOR];
	b2Vec2 position;
	int currentFrame;
	float currentTime;
};

