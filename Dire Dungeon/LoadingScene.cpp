#include "LoadingScene.h"
#include "DireDungeon.h"

LoadingScene::LoadingScene(SceneManager* _manager) : Scene(_manager)
{
	input = InputManager::GetInstance(); // Get the input manager
	drawn = false;  
	nextTexture = 1;
	  
}




void LoadingScene::Load() {
    
	ResourceManager* resources = ResourceManager::GetInstance(); // Get the resource manager for loading
	loadingMusic = resources->Load<Music>("Sounds/Tracks/Menu Track - The House of Leaves.mp3");
	loadingSplash = new Sprite(resources->Load<GLTexture>("Textures/Backgrounds/loadingSplash.png"), glm::vec4(0, 0, 165, 48));	
	loadingSplash2 = new Sprite(resources->Load<GLTexture>("Textures/Backgrounds/loadingSplash2.png"), glm::vec4(0, 0, 165, 48));	
	
	
	AudioManager::PlayMusic(loadingMusic, true);

  // Get the camera
  Camera2D* camera = ((DireDungeon*)manager->TheGame())->GetCamera(); // Get the game's main camera
   // Reset the camera
  camera->SetPosition(glm::vec2(0.0F, 0.0F));
  camera->SetScale(1.0F);
 
  camera->Update();

  
}

void LoadingScene::Unload() {
 // delete world;
}

void LoadingScene::LoadTextures(int textureToLoad) {
	ResourceManager* resources = ResourceManager::GetInstance(); // Get the resource manager for loading
	  // load all the textures

	if (textureToLoad == 1) resources->Load<GLTexture>("Textures/Backgrounds/blank.png");
	else if (textureToLoad == 2) resources->Load<GLTexture>("Textures/Decorations/wooden_box.png");		
	else if (textureToLoad == 3) resources->Load<GLTexture>("Textures/Backgrounds/RepeatingBG.png");
	else if (textureToLoad == 4) resources->Load<GLTexture>("Textures/Backgrounds/UI.png");
	else if (textureToLoad == 5) resources->Load<GLTexture>("Textures/Triggers/doorClosed.png");
	else if (textureToLoad == 6) resources->Load<GLTexture>("Textures/Backgrounds/CreditScreen.png");
	else if (textureToLoad == 7) resources->Load<GLTexture>("Textures/Decorations/barrel.png");
	else if (textureToLoad == 8) resources->Load<GLTexture>("Textures/Triggers/doorOpen.png");
	else if (textureToLoad == 9) resources->Load<GLTexture>("Textures/Backgrounds/StartScreen.png");
	else if (textureToLoad == 10) resources->Load<GLTexture>("Textures/Traps/log1.png");
	else if (textureToLoad == 11) resources->Load<GLTexture>("Textures/Traps/log2.png");
	else if (textureToLoad == 12) resources->Load<GLTexture>("Textures/Traps/rope.png");
	else if (textureToLoad == 13) resources->Load<GLTexture>("Textures/Character/WHITE-BOX.png");
	else if (textureToLoad == 14) resources->Load<GLTexture>("Textures/Character/happy.png");
	else if (textureToLoad == 15) resources->Load<GLTexture>("Textures/Character/surprise.png");
	else if (textureToLoad == 16) resources->Load<GLTexture>("Textures/Traps/ironMaiden1.png");
	else if (textureToLoad == 17) resources->Load<GLTexture>("Textures/Traps/ironMaiden2.png");
	else if (textureToLoad == 18) resources->Load<GLTexture>("Textures/Traps/ironMaiden3.png");
	else if (textureToLoad == 19) resources->Load<GLTexture>("Textures/Traps/ironMaiden4.png");
	else if (textureToLoad == 20) resources->Load<GLTexture>("Textures/Traps/ironMaiden5.png");
	else if (textureToLoad == 21) resources->Load<GLTexture>("Textures/Traps/ironMaiden6.png");
	else if (textureToLoad == 22) resources->Load<GLTexture>("Textures/Decorations/torch1.png");
	else if (textureToLoad == 23) resources->Load<GLTexture>("Textures/Decorations/torch2.png");
	else if (textureToLoad == 24) resources->Load<GLTexture>("Textures/Decorations/torch3.png");
	else if (textureToLoad == 25) resources->Load<GLTexture>("Textures/Traps/guillotineBlade.png");
	else if (textureToLoad == 26) resources->Load<GLTexture>("Textures/Traps/pendulum.png");
	else if (textureToLoad == 27) resources->Load<Music>("Sounds/Tracks/Replays - Hyperfun.mp3");
	else if (textureToLoad == 28) resources->Load<Sound>("Sounds/SFX/door-slam.wav");
	else if (textureToLoad == 29) resources->Load<Sound>("Sounds/SFX/grunt.ogg");
	else if (textureToLoad == 30) resources->Load<GLTexture>("Textures/Backgrounds/SelectedBorder.png");
	else if (textureToLoad == 31) resources->Load<Music>("Sounds/Tracks/Level Complete - Winner Winner.mp3");
	else if (textureToLoad == 32) resources->Load<Sound>("Sounds/SFX/guillotine.wav");	
	
	else  manager->ChangeScene("Title");	

	drawn = false;
	nextTexture++;

}

void LoadingScene::Update(GameTime& _gameTime) {
  const float scaleSpeed = 0.1F;  

  // Get the camera
  Camera2D* camera = ((DireDungeon*)manager->TheGame())->GetCamera(); // Get the game's main camera
   // Reset the camera
  camera->SetPosition(glm::vec2(0.0F, 0.0F));
  camera->SetScale(1.0F);
 
  camera->Update();
  
  /*if (input->KeyPress(SDLK_l) || input->KeyPress(SDLK_RETURN) )
    manager->ChangeScene("Title");*/
  if (drawn) {
	  LoadTextures(nextTexture);
  }

  

}

void LoadingScene::Render(SpriteBatch& _spriteBatch) {
//  world->Draw(_spriteBatch);
	(nextTexture%2 == 0) ? loadingSplash->Draw(_spriteBatch) : loadingSplash2->Draw(_spriteBatch) ;
	if (!drawn) drawn = true;
}