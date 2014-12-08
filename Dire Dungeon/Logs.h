#pragma once
#include "LogContainer.h"

class Logs : public LogContainer {
public:
	Logs(b2Vec2 _position, GameWorld* _world, GLTexture _texture);
	~Logs(void);

	void OpenDoor();
	bool isOpen;
	float triggerTime;

	void Draw(SpriteBatch& _spriteBatch);
private:
	GLTexture logTexture;
	float radius;
};