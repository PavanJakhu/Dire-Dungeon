#pragma once
#include <Direngine/GameWorld.h>

class Box : public GameObject {
public:
  void Draw(SpriteBatch& _spriteBatch);

  Box(b2Vec2 _position, GameWorld* _world, GLTexture _texture);
private:
  GLTexture texture;
};