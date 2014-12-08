#include "Torch.h"
#include "DireDungeon.h"

Torch::Torch(b2Vec2 _position)
{	
	position = _position;
	ResourceManager* resources = ResourceManager::GetInstance();
  
	torch[0] = new Sprite(resources->Load<GLTexture>("Textures/Decorations/torch1.png"), glm::vec4(_position.x, _position.y, 51, 90));  
	torch[1] = new Sprite(resources->Load<GLTexture>("Textures/Decorations/torch2.png"), glm::vec4(_position.x, _position.y, 51, 90));
  	torch[2] = new Sprite(resources->Load<GLTexture>("Textures/Decorations/torch3.png"), glm::vec4(_position.x, _position.y, 51, 90));

	currentFrame = 0;
	currentTime = 0;

}


Torch::~Torch(void)
{
}

void Torch::Update(GameTime& _gameTime) {
	currentTime += _gameTime.deltaTime;
	if (currentTime > 0.2f) {
		currentTime = 0;
		currentFrame = (currentFrame+1)%SPRITES_IN_CURSOR;
	}
}

void Torch::Move(b2Vec2 _velocity) {
	for (int i = 0 ; i < SPRITES_IN_CURSOR; i++) {
		torch[i]->Move(_velocity.x, _velocity.y);
	}
}

b2Vec2 Torch::GetPosition() {
	return position;
}

void Torch::Render(SpriteBatch& _spriteBatch) {

	torch[currentFrame]->Draw(_spriteBatch);

}
