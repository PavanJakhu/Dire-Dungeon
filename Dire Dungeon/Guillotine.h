#pragma once
#include <Direngine\GameWorld.h>
#include <Direngine\AudioManager.h>
class Guillotine :
	public GameObject
{
public:
	Guillotine(b2Vec2 _position, GameWorld* _world, GLTexture _ropeTexture, GLTexture _bladeTexture);
	~Guillotine();

	void Draw(SpriteBatch& _spriteBatch);

	void Fall();
	b2Vec2 ApplyForce(GameTime _time);
	float triggerTime;
	bool hasFallen;

	//Getter and Setter
	const void setAnchorPos(const b2Vec2& _anchorPos) { anchorPos = _anchorPos; }
	const b2Vec2 getAnchorPos() { return anchorPos; }

private:
	GLTexture ropeTexture;
	GLTexture bladeTexture;
	GLTexture guardTexture;

	const int NUM_SEGMENTS;
	const int NUM_JOINTS;
	const int NUM_ROPE_JOINTS;
	const float ROPE_WIDTH;
	const float ROPE_HEIGHT;
	const float BLADE_WIDTH;
	const float BLADE_HEIGHT;

	b2Vec2 anchorPos;
	std::vector<b2Joint*> ropeJoints;

	Sound guillotineSound;

	void ChangeFilters(b2Body* body, int index);
};

