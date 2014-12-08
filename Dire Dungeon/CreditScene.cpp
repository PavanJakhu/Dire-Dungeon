#include "CreditScene.h"
#include "DireDungeon.h"

CreditScene::CreditScene(SceneManager* _manager) : Scene(_manager) {
  // world = nullptr;
  input = InputManager::GetInstance(); // Get the input manager
}

void CreditScene::Load() {
  
  ResourceManager* resources = ResourceManager::GetInstance(); // Get the resource manager for loading

  creditSplash = new Sprite(resources->Load<GLTexture>("Textures/Backgrounds/CreditScreen.png"), glm::vec4(0, 0, 800, 640));

  // Get the camera
  Camera2D* camera = ((DireDungeon*)manager->TheGame())->GetCamera(); // Get the game's main camera
   // Reset the camera
  camera->SetPosition(glm::vec2(0.0F, 0.0F));
  camera->SetScale(1.0F);
  camera->Update();

}

void CreditScene::Unload() {
 // delete world;
}

void CreditScene::Update(GameTime& _gameTime) {
  const float scaleSpeed = 0.1F;

  //if (input->KeyPress(SDLK_d))
  //  ball->GetBody()->ApplyForceToCenter(b2Vec2(40.0F, 60.0F), true);

  /*if (input->KeyDown(SDLK_d))
    test->GetBody(1)->ApplyForceToCenter(b2Vec2(0.0F, 10.0F), true);
  if (input->KeyDown(SDLK_a))
    test->GetBody(0)->ApplyForceToCenter(b2Vec2(0.0F, 10.0F), true);*/


  // Tester for reloading scene
  if (input->KeyPress(SDLK_l) || input->KeyPress(SDLK_RETURN) || input->KeyPress(SDLK_ESCAPE) )
    manager->ChangeScene("Title");

  
 // world->Update(_gameTime);
}

void CreditScene::Render(SpriteBatch& _spriteBatch) {
//  world->Draw(_spriteBatch);
	creditSplash->Draw(_spriteBatch);
}