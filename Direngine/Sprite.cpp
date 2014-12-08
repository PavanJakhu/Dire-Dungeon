#include "Sprite.h"
#include "Vertex.h"

void Sprite::Update(GameTime& _gameTime) {

}

void Sprite::Draw(SpriteBatch& _spriteBatch) {
  glm::vec4 pos(bounds.x, bounds.y, bounds.z, bounds.w);
  glm::vec4 uv(0.0F, 0.0F, 1.0F, 1.0F);
  Color color;

  _spriteBatch.Draw(pos, uv, texture.id, 0.0F, 0.0F, color);
}

void Sprite::Move(float velocity_x, float velocity_y) {
	bounds.x += velocity_x;
	bounds.y += velocity_y;
}

glm::vec2 Sprite::GetPosition() {
	return glm::vec2(bounds.x, bounds.y);
}

glm::vec2 Sprite::GetDimensions() {
	return glm::vec2(bounds.z, bounds.w);
}