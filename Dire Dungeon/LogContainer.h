#pragma once
#include <Direngine/GameWorld.h>

class LogContainer : public GameObject {
public:
	LogContainer(b2Vec2 _position, GameWorld* _world);
	~LogContainer(void);

	void Draw(SpriteBatch& _spriteBatch);
protected:
	float x, y, width, height;
	b2Joint* jointOne;
private:
	GLTexture containerTexture;
};