#pragma once
#include <Direngine/GameWorld.h>
#include <Direngine/Sprite.h>

const int SPRITES_IN_ANIMATION = 6;

class IronMaiden :
	public GameObject
{
public:
	IronMaiden(float _scale, glm::vec4 _bounds, GameWorld* _world);
	~IronMaiden(void);
	void Trigger();
	bool ApplyForce();
	void Draw(SpriteBatch& _spriteBatch);
	bool triggered;
	float triggerTime;
protected:	
	glm::vec4 bounds;
	int currentTexture;
	b2Joint* jointOne;
private:
	GLTexture* texture;
	float scale;
	Sprite* spriteArray[SPRITES_IN_ANIMATION];
	bool animating;
	bool forwardAnimating;
	bool forceApplied;
};

