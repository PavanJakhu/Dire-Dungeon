#include "Box.h"

Box::Box(b2Vec2 _position, GameWorld* _world, GLTexture _texture) : GameObject(_world), texture(_texture) {
  b2BodyDef bd;
  bd.type = b2_dynamicBody;
  bd.position = _position;

  bodies.push_back(world->GetB2World()->CreateBody(&bd));

  b2PolygonShape box;
  box.SetAsBox(0.25F, 0.25F); // 1m^2 box

  b2FixtureDef fd;
  fd.shape = &box;
  fd.density = 1.0F;
  fd.friction = 0.1F;
  GetBody()->SetUserData((void*)-2);
  GetBody()->CreateFixture(&fd);
}

void Box::Draw(SpriteBatch& _spriteBatch) {
  // Get position in world coordinates, in metres
  b2Vec2 positionMetres = GetBody()->GetPosition();
  // Hardcoded because Box2D is fucking retarded and I have no way of finding the width/height in metres from the body
  float widthMetres = 0.5F, heightMetres = 0.5F;
  // Scale it to 100 pixels per metre
  glm::vec4 destRect(positionMetres.x * 100, positionMetres.y * 100, widthMetres * 100, heightMetres * 100);

  glm::vec4 uv(0.0F, 0.0F, 1.0F, 1.0F);
  Color color;
  color.r = 255; color.g = 255; color.b = 255; color.a = 255;

  _spriteBatch.Draw(destRect, uv, texture.id, 0.0F, GetBody()->GetAngle(), color);
}