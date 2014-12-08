#include "LogTestingScene.h"
#include "DireDungeon.h"
#include "Ground.h"

LogTestingScene::LogTestingScene(SceneManager* _manager) : Scene(_manager)
{
	world = nullptr;
	input = InputManager::GetInstance(); // Get the input manager
	camera = ((DireDungeon*)manager->TheGame())->GetCamera(); // Get the game's main camera
}


LogTestingScene::~LogTestingScene(void)
{
}

void LogTestingScene::Load()
{
	world = new GameWorld(b2Vec2(0.0F, -9.8F));

	ResourceManager* resources = ResourceManager::GetInstance(); // Get the resource manager for loading

	//world->AddObject(logs = new Logs(world, resources->Load<GLTexture>("Textures/Traps/log1.png")));

	// "Ground"
	//world->AddObject(new Ground(b2Vec2(0.0F, -3.0F), world->GetB2World(), resources->Load<GLTexture>("Textures/jimmyJump/PNG/CharacterRight_Standing.png")));

	// Reset the camera
	camera->SetPosition(glm::vec2(0.0F, 0.0F));
	camera->SetScale(1.0F);
}

void LogTestingScene::Unload()
{
	delete world;
}

void LogTestingScene::Update(GameTime& _gameTime)
{
	camera->Update();
	world->Update(_gameTime);

	if (input->MouseButtonPress(SDL_MOUSEBUTTONDOWN))
	{
		logs->OpenDoor();
	}
}

void LogTestingScene::Render(SpriteBatch& _spriteBatch)
{
	world->Draw(_spriteBatch);
}