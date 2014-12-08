#pragma once
#include <Direngine/GameWorld.h>

class Ground : public GameObject {
public:
	void Draw(SpriteBatch& _spriteBatch);

	Ground(b2Vec2 _position, GameWorld* _world, GLTexture _texture);
private:
  GLTexture texture;
};