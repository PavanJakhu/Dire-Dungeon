#include "Level1Scene.h"

#include "DireDungeon.h"

Level1Scene::Level1Scene(SceneManager* _manager) : Scene(_manager) {
	world = nullptr;
	input = InputManager::GetInstance(); // Get the input manager
	camera = ((DireDungeon*)manager->TheGame())->GetCamera(); // Get the game's main camera
}

void Level1Scene::Load() {
	world = new GameWorld(b2Vec2(0.0F, -9.8F));

	ResourceManager* resources = ResourceManager::GetInstance(); // Get the resource manager for loading

	world->AddObject(t1 = new RagdollCharacter(b2Vec2(0.0f,0.0f), world, resources));

	// "Wooden Boxes"
	world->AddObject(new Box(b2Vec2(0.0F, 2.5F), world, resources->LoadTexture("Textures/assign4/wooden_box.png")));
	world->AddObject(t2 = new Box(b2Vec2(0.0F, 3.5F), world, resources->LoadTexture("Textures/assign4/wooden_box.png")));
	world->AddObject(new Box(b2Vec2(0.0F, 4.5F), world, resources->LoadTexture("Textures/assign4/wooden_box.png")));
	// "Ground"
	world->AddObject(new Ground(b2Vec2(0.0F, -3.0F), world, resources->LoadTexture("Textures/jimmyJump/PNG/CharacterRight_Standing.png")));

	// Reset the camera
	camera->SetPosition(glm::vec2(0.0F, 0.0F));
	camera->SetScale(1.0F);
}

void Level1Scene::Unload() {
	delete world;
}

void Level1Scene::Update(GameTime& _gameTime) {
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
}

void Level1Scene::Render(SpriteBatch& _spriteBatch) {
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