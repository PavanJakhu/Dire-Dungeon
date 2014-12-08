#pragma once
#include <GLM/glm.hpp>
#include "GLTexture.h"
#include "SpriteBatch.h"
#include "GameTime.h"

class Sprite {
public:
  virtual void Update(GameTime& _gameTime);
  virtual void Draw(SpriteBatch& _spriteBatch);

  void Move(float velocity_x, float velocity_y);
  glm::vec2 GetPosition();
  glm::vec2 GetDimensions();

  Sprite(GLTexture _texture, const glm::vec4& _bounds) : texture(_texture), bounds(_bounds) {}
  ~Sprite() {}

 
protected:
  GLTexture texture;
  glm::vec4 bounds;
  bool movesWithBackground;
};