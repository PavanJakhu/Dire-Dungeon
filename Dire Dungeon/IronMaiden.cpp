#include "IronMaiden.h"
#include "DireDungeon.h"

#include <iostream>


IronMaiden::IronMaiden(float _scale, glm::vec4 _bounds, GameWorld* _world) : GameObject(_world)
{
	bounds.x = _bounds.x; // x
	bounds.y = _bounds.y; // y
	bounds.z = _bounds.z; // width
	bounds.w = _bounds.w; // height
	scale = _scale;
	currentTexture = 0;
	triggerTime = 0.0f;

	ResourceManager* resources = ResourceManager::GetInstance(); // Get the resource manager for loading
	spriteArray[0] = new Sprite(resources->Load<GLTexture>("Textures/Traps/ironMaiden1.png"), glm::vec4(bounds.x, bounds.y, 48*scale, 61*scale));
	spriteArray[1] = new Sprite(resources->Load<GLTexture>("Textures/Traps/ironMaiden2.png"), glm::vec4(bounds.x, bounds.y, 48*scale, 61*scale));
	spriteArray[2] = new Sprite(resources->Load<GLTexture>("Textures/Traps/ironMaiden3.png"), glm::vec4(bounds.x, bounds.y, 48*scale, 61*scale));
	spriteArray[3] = new Sprite(resources->Load<GLTexture>("Textures/Traps/ironMaiden4.png"), glm::vec4(bounds.x, bounds.y, 48*scale, 61*scale));
	spriteArray[4] = new Sprite(resources->Load<GLTexture>("Textures/Traps/ironMaiden5.png"), glm::vec4(bounds.x, bounds.y, 48*scale, 61*scale));
	spriteArray[5] = new Sprite(resources->Load<GLTexture>("Textures/Traps/ironMaiden6.png"), glm::vec4(bounds.x, bounds.y, 48*scale, 61*scale));

	animating = false;
	forwardAnimating = true;
	triggered = false;
	
}


IronMaiden::~IronMaiden(void)
{
}

void IronMaiden::Trigger() {

	triggered = true;
	animating = true;
	forwardAnimating = true;
	//Head
		
}

bool IronMaiden::ApplyForce() {

	if (animating && !forwardAnimating && !forceApplied) {
		forceApplied = true;
		return true;
	}
	return false;
}

void IronMaiden::Draw(SpriteBatch& _spriteBatch) {
	
	if (animating) { // if animating the sprite
		spriteArray[currentTexture]->Draw(_spriteBatch); // draw current sprite
		
		if(forwardAnimating) {  // if cycling forward
			if (currentTexture == SPRITES_IN_ANIMATION-1)  // reverse flow!!
				forwardAnimating = false;			
			else
				currentTexture++;
		}
		else { // if cycling backward
			currentTexture--;
		}

		if (currentTexture == 0) {
			animating = false;
		}
	}

	else {
		spriteArray[0]->Draw(_spriteBatch);
		if (forceApplied) forceApplied = false;
	}
}
