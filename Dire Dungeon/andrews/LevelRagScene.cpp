#include "Level1Scene.h"

#include "DireDungeon.h"
#include <iostream>

float scaleSpeed = 0.1F;
float bgSpeed = 3.0F;

Level1Scene::Level1Scene(SceneManager* _manager) : Scene(_manager) {
	world = nullptr;
	input = InputManager::GetInstance(); // Get the input manager
	camera = ((DireDungeon*)manager->TheGame())->GetCamera(); // Get the game's main camera
}

void Level1Scene::Load() {
	world = new GameWorld(b2Vec2(0.0F, -9.8F));

<<<<<<< HEAD
	ResourceManager* resources = ResourceManager::GetInstance(); // Get the resource manager for loading

	world->AddObject(t1 = new RagdollCharacter(b2Vec2(0,0), world, resources));

	// "Wooden Boxes"
	world->AddObject(new Box(b2Vec2(0.0F, 2.5F), world, resources->LoadTexture("Textures/assign4/wooden_box.png")));
	world->AddObject(t2 = new Box(b2Vec2(0.0F, 3.5F), world, resources->LoadTexture("Textures/assign4/wooden_box.png")));
	world->AddObject(new Box(b2Vec2(0.0F, 4.5F), world, resources->LoadTexture("Textures/assign4/wooden_box.png")));
	// "Ground"
	world->AddObject(new Ground(b2Vec2(0.0F, -3.0F), world, resources->LoadTexture("Textures/jimmyJump/PNG/CharacterRight_Standing.png")));
=======
  ResourceManager* resources = ResourceManager::GetInstance(); // Get the resource manager for loading 
  sprites[0] = new Sprite(resources->LoadTexture("Textures/Backgrounds/RepeatingBG.png"), glm::vec4(0, 0, 800, 640));
  sprites[1] = new Sprite(resources->LoadTexture("Textures/Backgrounds/RepeatingBG.png"), glm::vec4(800, 0, 800, 640));
  sprites[2] = new Sprite(resources->LoadTexture("Textures/Backgrounds/RepeatingBG.png"), glm::vec4(1600, 0, 800, 640));
  sprites[3] = new Sprite(resources->LoadTexture("Textures/Backgrounds/UI.png"), glm::vec4(0, -250, 800, 126));
  sprites[4] = new Sprite(resources->LoadTexture("Textures/Triggers/doorClosed.png"), glm::vec4(-200, -76, 72, 99));
  sprites[5] = new Sprite(resources->LoadTexture("Textures/Triggers/doorOpen.png"), glm::vec4(1800, -76, 72, 99));
  sprites[6] = new Sprite(resources->LoadTexture("Textures/Decorations/barrel.png"), glm::vec4(600, -100, 48, 54));
   
  
  // "Wooden Boxes"
  world->AddObject(new Box(b2Vec2(0.0F, 2.5F), world, resources->LoadTexture("Textures/Decorations/wooden_box.png")));
  world->AddObject(new Box(b2Vec2(7.5F, 3.5F), world, resources->LoadTexture("Textures/Decorations/wooden_box.png")));
  world->AddObject(new Box(b2Vec2(15.0F, 4.5F), world, resources->LoadTexture("Textures/Decorations/wooden_box.png")));
  // "Ground"
  world->AddObject(new Ground(b2Vec2(0.0F, -1.25F), world, resources->LoadTexture("Textures/Backgrounds/blank.png"))); 
  // "Ceiling"
  world->AddObject(new Wall(b2Vec2(0.0F, 3.0F), world, resources->LoadTexture("Textures/Decorations/barrel.png"),false)); 
  // Walls
  world->AddObject(new Wall(b2Vec2(-4.0F, 0.0F), world, resources->LoadTexture("Textures/Decorations/barrel.png"), true)); // left
  world->AddObject(new Wall(b2Vec2(20.0F, 0.0F), world, resources->LoadTexture("Textures/Decorations/barrel.png"), true)); 
>>>>>>> origin/master

	// Reset the camera
	camera->SetPosition(glm::vec2(0.0F, 0.0F));
	camera->SetScale(1.0F);
}

void Level1Scene::Unload() {
	delete world;
}

void Level1Scene::Update(GameTime& _gameTime) {
<<<<<<< HEAD
	const float scaleSpeed = 0.1F;

	//if (input->KeyPress(SDLK_d))
	//  ball->GetBody()->ApplyForceToCenter(b2Vec2(40.0F, 60.0F), true);

	if (input->KeyDown(SDLK_d))
		t1->GetBody(1)->ApplyForceToCenter(b2Vec2(0.0F, 10.0F), true);
	if (input->KeyDown(SDLK_a))
		t1->GetBody(0)->ApplyForceToCenter(b2Vec2(0.0F, 10.0F), true);

	// Tester for reloading scene
	if (input->KeyPress(SDLK_l))
		manager->ChangeScene("Level1");

	// Tester for removing objects
	if (input->KeyPress(SDLK_k))
		t2->Destroy();

	camera->Update();
	world->Update(_gameTime);
=======
     

  /*if (input->KeyDown(SDLK_q))
    camera->SetScale(camera->GetScale() + scaleSpeed);
  if (input->KeyDown(SDLK_e))
    camera->SetScale(camera->GetScale() - scaleSpeed);*/

  // moving background
  if (input->KeyDown(SDLK_a)) {
	  UpdateCamera(-bgSpeed);
  }

  if (input->KeyDown(SDLK_d)) {
	   UpdateCamera(bgSpeed);
  }
	 
  // Tester for reloading scene
  if (input->KeyPress(SDLK_l))
    manager->ChangeScene("Level1");

  camera->Update();
  world->Update(_gameTime);
>>>>>>> origin/master
}


void Level1Scene::Render(SpriteBatch& _spriteBatch) {
<<<<<<< HEAD
	world->Draw(_spriteBatch);
}

void MyContactListener::BeginContact(b2Contact* contact) {

	//check if fixture A was a ball
	void* bodyUserData1 = contact->GetFixtureA()->GetBody()->GetUserData();
	//check if fixture B was a ball
	void* bodyUserData2 = contact->GetFixtureB()->GetBody()->GetUserData();

	if( (bodyUserData1 && ((int)bodyUserData2 == 1)) || (bodyUserData2 && ((int)bodyUserData1 == 1)) )
	{
		static_cast<RagdollCharacter*> (bodyUserData1)->beginContact();
	}

}

void MyContactListener::EndContact(b2Contact* contact) 
{

	//check if fixture A was a ball
	void* bodyUserData1 = contact->GetFixtureA()->GetBody()->GetUserData();
	//check if fixture B was a ball
	void* bodyUserData2 = contact->GetFixtureB()->GetBody()->GetUserData();

	if( (bodyUserData1 && ((int)bodyUserData2 == 1)) || (bodyUserData2 && ((int)bodyUserData1 == 1)) )
	{
		static_cast<RagdollCharacter*> (bodyUserData1)->endContact();
	}
}
=======
	for (int i = 0; i< SPRITENUMBER; i++) {
		if (camera->IsInView(sprites[i]->GetPosition(),sprites[i]->GetDimensions())) {
			sprites[i]->Draw(_spriteBatch);
		}
	}
  world->Draw(_spriteBatch);
}

void Level1Scene::UpdateCamera(float xVelocity) {
	
	// moving background	
	camera->SetPosition(glm::vec2(camera->GetPosition().x + xVelocity, camera->GetPosition().y ));
	sprites[3]->Move(xVelocity, 0.0); // move UI layer
  

   if ( camera->GetPosition().x > 1600) {
	  camera->SetPosition(glm::vec2(1600, camera->GetPosition().y ));
	  sprites[3]->Move(-xVelocity, 0.0); // move UI layer
   }

   if (camera->GetPosition().x < 0 ) {
	  camera->SetPosition(glm::vec2(0, camera->GetPosition().y ));
	  sprites[3]->Move(-xVelocity, 0.0); // move UI layer
   }
  
}
>>>>>>> origin/master
