#include "TitleScene.h"

#include "DireDungeon.h"

TitleScene::TitleScene(SceneManager* _manager) : Scene(_manager) {
  // world = nullptr;
  input = InputManager::GetInstance(); // Get the input manager
  
}

void TitleScene::Load() {
  
  ResourceManager* resources = ResourceManager::GetInstance(); // Get the resource manager for loading

  titleSplash = new Sprite(resources->Load<GLTexture>("Textures/Backgrounds/StartScreen.png"), glm::vec4(0, 0, 800, 640));
  torch = new Torch(b2Vec2(-280,-230));
  doorSlam = resources->Load<Sound>("Sounds/SFX/door-slam.wav");
  titleMusic = resources->Load<Music>("Sounds/Tracks/Menu Track - The House of Leaves.mp3");
  
  AudioManager::PlayMusic(titleMusic, true);

  // Get the camera
  Camera2D* camera = ((DireDungeon*)manager->TheGame())->GetCamera(); // Get the game's main camera
   // Reset the camera
  camera->SetPosition(glm::vec2(0.0F, 0.0F));
  camera->SetScale(1.0F);
 
  camera->Update();

  currentCursorPosition = 0;
  

}

void TitleScene::Unload() {
 // delete world;
}

void TitleScene::Update(GameTime& _gameTime) {
  const float scaleSpeed = 0.1F;

	if (input->KeyPress(SDLK_l) || input->KeyPress(SDLK_RETURN) ) {
		AudioManager::PlaySound(doorSlam, 0);
	}

   	  switch (currentCursorPosition) {
		case 0:
			if (input->KeyPress(SDLK_d)|| input->KeyPress(SDLK_RIGHT)) {
				torch->Move(b2Vec2(200,0));
				currentCursorPosition++;
			}
			if (input->KeyPress(SDLK_l) || input->KeyPress(SDLK_RETURN) )
				manager->ChangeScene("Level1");
			break;
		case 1:
			if (input->KeyPress(SDLK_d)|| input->KeyPress(SDLK_RIGHT)) {
				torch->Move(b2Vec2(190,0));
				currentCursorPosition++;
			}
			if (input->KeyPress(SDLK_a)|| input->KeyPress(SDLK_LEFT)) {
				torch->Move(b2Vec2(-200,0));
				currentCursorPosition--;
			}
			if (input->KeyPress(SDLK_l) || input->KeyPress(SDLK_RETURN) )
				manager->ChangeScene("Level1");
			break;
		case 2:
			if (input->KeyPress(SDLK_d)|| input->KeyPress(SDLK_RIGHT)) {
				torch->Move(b2Vec2(160,0));
				currentCursorPosition++;
			}
			if (input->KeyPress(SDLK_a)|| input->KeyPress(SDLK_LEFT)) {
				torch->Move(b2Vec2(-190,0));
				currentCursorPosition--;
			}
			if (input->KeyPress(SDLK_l) || input->KeyPress(SDLK_RETURN) )
				manager->ChangeScene("Credits");
			break;
		case 3:			
			if (input->KeyPress(SDLK_a)|| input->KeyPress(SDLK_LEFT)) {
				torch->Move(b2Vec2(-160,0));
				currentCursorPosition--;
			}
			if (input->KeyPress(SDLK_l) || input->KeyPress(SDLK_RETURN) ) {
				SDL_Quit();
				exit(1);
			}
			break;
	  }
	  
  
	  torch->Update(_gameTime);

  /*if (input->KeyDown(SDLK_d))
    test->GetBody(1)->ApplyForceToCenter(b2Vec2(0.0F, 10.0F), true);
  if (input->KeyDown(SDLK_a))
    test->GetBody(0)->ApplyForceToCenter(b2Vec2(0.0F, 10.0F), true);*/

  // Get the camera
  Camera2D* camera = ((DireDungeon*)manager->TheGame())->GetCamera(); // Get the game's main camera
   // Reset the camera
  camera->SetPosition(glm::vec2(0.0F, 0.0F));
  camera->SetScale(1.0F);
 
  camera->Update();


  // Tester for reloading scene
  /*if (input->KeyPress(SDLK_l) || input->KeyPress(SDLK_RETURN) )
    manager->ChangeScene("Level1");

  if (input->KeyPress(SDLK_c) )
    manager->ChangeScene("Credits");
  */

  
 // world->Update(_gameTime);
}

void TitleScene::Render(SpriteBatch& _spriteBatch) {
//  world->Draw(_spriteBatch);
	titleSplash->Draw(_spriteBatch);
	torch->Render(_spriteBatch);
}