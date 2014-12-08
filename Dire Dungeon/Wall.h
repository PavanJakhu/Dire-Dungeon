#pragma once
#include <Direngine/GameWorld.h>

class Wall : public GameObject {
public:
	void Draw(SpriteBatch& _spriteBatch);

	Wall(b2Vec2 _position, GameWorld* _world, GLTexture _texture, float _width, float _height);
private:
  GLTexture texture;
};