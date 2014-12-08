#pragma once
#include <Direngine/GameWorld.h>

class Ball : public GameObject {
public:
  void Draw(SpriteBatch& _spriteBatch);

  Ball(b2Vec2 _position, GameWorld* _world, GLTexture _texture);
private:
  GLTexture texture;
};