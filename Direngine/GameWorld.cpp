#include "GameWorld.h"

GameWorld::GameWorld(b2Vec2 _gravity) {
  world = new b2World(_gravity);
}

GameWorld::~GameWorld() {
  while (objects.size()) {
    delete objects.back();
    objects.pop_back();
  }

  delete world;
}

void GameWorld::AddObject(GameObject* _gameObject) {
  objects.push_back(_gameObject);
}

void GameWorld::DestroyObject(GameObject* _gameObject) {
  // Get the iterator for the object in the vector
  auto object = std::find(objects.begin(), objects.end(), _gameObject);

  // If the object was not found, return
  if (object == objects.end()) return;

  // Delete the object
  delete _gameObject;
  // Then erase the object from the vector
  objects.erase(object);
}

void GameWorld::Update(GameTime& _gameTime) {
  if (objects.size() > 0)
    for (int i = objects.size() - 1; i >= 0; i--)
      objects[i]->Update(_gameTime);

  world->Step(_gameTime.deltaTime, 6, 2);
}

void GameWorld::Draw(SpriteBatch& _spriteBatch) {
  if (objects.size() > 0)
    for (int i = objects.size() - 1; i >= 0; i--)
      objects[i]->Draw(_spriteBatch);
}

GameObject::~GameObject() {
  while (bodies.size()) {
    world->GetB2World()->DestroyBody(bodies.back());
    bodies.pop_back();
  }
}

b2Body* GameObject::GetBody(int _body) {
  return bodies[_body];
}

b2Joint* GameObject::GetJoint(int _joint) {
  return joints[_joint];
}

int GameObject::GetBodyCount() {
  return bodies.size();
}

int GameObject::GetJointCount() {
  return joints.size();
}

void GameObject::Destroy() {
  world->DestroyObject(this);
}