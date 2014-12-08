#include "Level1Scene.h"

#include "DireDungeon.h"

#include "Box.h"
#include "Ball.h"
#include "Ground.h"
#include "Wall.h"

#include <Direngine/Errors.h>

float scaleSpeed = 0.1F;


Level1Scene::Level1Scene(SceneManager* _manager) : Scene(_manager) {
  world = nullptr;

  input = InputManager::GetInstance(); // Get the input manager
  camera = ((DireDungeon*)manager->TheGame())->GetCamera(); // Get the game's main camera
  characterSpeed = 0.02f;
  currentLevel = 0;
  score = 1924;
  
}

void Level1Scene::Load() {
  world = new GameWorld(b2Vec2(0.0F, -9.8F));
  myContactListenerInstance = new MyContactListener(this); // Pass MyContactListener a pointer to Level1Scene, which it is a friend of, allowing it access to variables
  world->GetB2World()->SetContactListener(myContactListenerInstance);
  trapsHit = 0;

  ResourceManager* resources = ResourceManager::GetInstance(); // Get the resource manager for loading 
  sprites[0] = new Sprite(resources->Load<GLTexture>("Textures/Backgrounds/RepeatingBG.png"), glm::vec4(0, 0, 800, 640));
  sprites[1] = new Sprite(resources->Load<GLTexture>("Textures/Backgrounds/RepeatingBG.png"), glm::vec4(800, 0, 800, 640));
  sprites[2] = new Sprite(resources->Load<GLTexture>("Textures/Backgrounds/RepeatingBG.png"), glm::vec4(1600, 0, 800, 640));
  sprites[3] = new Sprite(resources->Load<GLTexture>("Textures/Backgrounds/RepeatingBG.png"), glm::vec4(2400, 0, 800, 640));
  sprites[4] = new Sprite(resources->Load<GLTexture>("Textures/Backgrounds/UI.png"), glm::vec4(0, -250, 800, 126));
  sprites[5] = new Sprite(resources->Load<GLTexture>("Textures/Triggers/doorClosed.png"), glm::vec4(-200, -76, 72, 99));
  sprites[6] = new Sprite(resources->Load<GLTexture>("Textures/Triggers/doorOpen.png"), glm::vec4(1800, -76, 72, 99)); 
  sprites[7] = new Sprite(resources->Load<GLTexture>("Textures/Decorations/barrel.png"), glm::vec4(600, -100, 48, 54));
  sprites[8] = new Sprite(resources->Load<GLTexture>("Textures/Backgrounds/SelectedBorder.png"),glm::vec4(-337, -252, 85, 93)); // UI Border  
  
 
  // Static Objects
  // "Ground"
  world->AddObject(new Ground(b2Vec2(0.0F, -1.25F), world, resources->Load<GLTexture>("Textures/Backgrounds/blank.png"))); 
  // "Ceiling"
  world->AddObject(new Wall(b2Vec2(0.0F, 3.2F), world, resources->Load<GLTexture>("Textures/Decorations/barrel.png") ,80.0f ,0.2f )); 
  // Walls
  world->AddObject(new Wall(b2Vec2(-4.0F, 0.9F), world, resources->Load<GLTexture>("Textures/Backgrounds/blank.png"), 0.5f, 2.0f)); // left
  world->AddObject(new Wall(b2Vec2(20.0F, 0.9F), world, resources->Load<GLTexture>("Textures/Backgrounds/blank.png"), 0.5f, 2.0f)); 
  
  // Player Spawn
  // Player    
  world->AddObject(ragdoll = new RagdollCharacter(2.0f, characterSpeed, b2Vec2(-1.5f,-0.8F), world, resources));  

  //Random Positions for Traps
  float xPositions[5] = { 3.0f, 6.0f, 9.0f,12.0f, 15.0f };
  float positionChosen[5] = {-1, -1, -1, -1, -1};  
  int i = 0;
  while (i < 5) {	  
	  int randomPosition = rand() % 5;
	  if (positionChosen[i] == -1 && xPositions[randomPosition] != -1) {
		  positionChosen[i] = xPositions[randomPosition];
		  xPositions[randomPosition] = -1;
		  i++;
	  }	  
  }
  
  // Spawn Traps 
  // Log Trap
  world->AddObject(logs = new Logs(b2Vec2(positionChosen[0], 1.0F), world, resources->Load<GLTexture>("Textures/Traps/log1.png")));
  // Guillotine
  world->AddObject(guillotine = new Guillotine(b2Vec2(positionChosen[1], 3.4f), world, resources->Load<GLTexture>("Textures/Traps/rope.png"),resources->Load<GLTexture>("Textures/Traps/guillotineBlade.png")));
  //Pendulum
  world->AddObject(pendulum = new Pendulum(b2Vec2(positionChosen[2], 1.3f), world, resources->Load<GLTexture>("Textures/Traps/pendulum.png")));    
  // IronMaiden
  world->AddObject(ironMaiden = new IronMaiden(2.2f,glm::vec4(positionChosen[3]*100,-60,2.0F,2.0F),world));
   //Ball and Chain
  world->AddObject(ballAndChain = new BallAndChain(b2Vec2(positionChosen[4], 1.0f), world, resources->Load<GLTexture>("Textures/Traps/ball.png"), resources->Load<GLTexture>("Textures/Traps/chain.png")));
  
  
  // Music
  winMusic = resources->Load<Music>("Sounds/Tracks/Level Complete - Winner Winner.mp3");
  levelMusic = resources->Load<Music>("Sounds/Tracks/Replays - Hyperfun.mp3");

  // Reset the camera
  camera->SetPosition(glm::vec2(0.0F, 0.0F));
  camera->SetScale(1.0F);
  // Play the music
  AudioManager::PlayMusic(levelMusic,true);
  // UI Selection
  currentUISelection = 0;
}

void Level1Scene::Unload() {
  if (world) {
    delete world;
    world = nullptr;
  }

  if (myContactListenerInstance) {
    delete myContactListenerInstance;
    myContactListenerInstance = nullptr;
  }
}

void Level1Scene::Update(GameTime& _gameTime) {

  time = _gameTime;

  float playerVelocity = ragdoll->playerVelocity();

  if(logs && logs->isOpen == false && (logs->triggerTime == 0.0f  || (logs->triggerTime + TRAP_DELETE_DELAY) < _gameTime.time))
  {
	  if(logs->triggerTime == 0)
	  {
		  logs->triggerTime = _gameTime.time;
	  }
	  else if ((logs->triggerTime + TRAP_DELETE_DELAY) < _gameTime.time)
	  {
		  world->DestroyObject(logs);
	  }
  }
  
  if(pendulum && pendulum->triggered == true && (pendulum->triggerTime == 0.0f  || (pendulum->triggerTime + TRAP_DELETE_DELAY*1.5) < _gameTime.time))
  {
	  if(pendulum->triggerTime == 0)
	  {
		  pendulum->triggerTime = _gameTime.time;
	  }
	  else if ((pendulum->triggerTime + TRAP_DELETE_DELAY) < _gameTime.time)
	  {
		  world->DestroyObject(pendulum);
	  }
  }

  if(guillotine && guillotine->hasFallen == true && (guillotine->triggerTime == 0.0f  || (guillotine->triggerTime + TRAP_DELETE_DELAY) < _gameTime.time))
  {
	  if(guillotine->triggerTime == 0)
	  {
		  guillotine->triggerTime = _gameTime.time;
	  }
	  else if ((guillotine->triggerTime + TRAP_DELETE_DELAY) < _gameTime.time)
	  {
		  world->DestroyObject(guillotine);
	  }
  }

  if(ironMaiden && ironMaiden->triggered == true && (ironMaiden->triggerTime == 0.0f  || (ironMaiden->triggerTime + TRAP_DELETE_DELAY) < _gameTime.time))
  {
	  if(ironMaiden->triggerTime == 0)
	  {
		  ironMaiden->triggerTime = _gameTime.time;
	  }
	  else if ((ironMaiden->triggerTime + TRAP_DELETE_DELAY) < _gameTime.time)
	  {
		  world->DestroyObject(ironMaiden);
	  }
  }

  if(ballAndChain && ballAndChain->triggered == true && (ballAndChain->triggerTime == 0.0f  || (ballAndChain->triggerTime + TRAP_DELETE_DELAY) < _gameTime.time))
  {
	  if(ballAndChain->triggerTime == 0)
	  {
		  ballAndChain->triggerTime = _gameTime.time;
	  }
	  else if ((ballAndChain->triggerTime + TRAP_DELETE_DELAY) < _gameTime.time)
	  {
		  world->DestroyObject(ballAndChain);
	  }
  }

  // select trap to activate
  switch (currentUISelection) {
  case 0: // Ball and Chain
    if (input->KeyPress(SDLK_d)|| input->KeyPress(SDLK_RIGHT)) {
      sprites[8]->Move(93,0);
      currentUISelection++;
    }
    if (input->KeyPress(SDLK_l) || input->KeyPress(SDLK_RETURN)) {     
      // trigger ball and chain
		ballAndChain->Trigger();
    }
    break;
  case 1: // Iron Maiden
    if (input->KeyPress(SDLK_d)|| input->KeyPress(SDLK_RIGHT)) {
      sprites[8]->Move(94,0);
      currentUISelection++;
    }
    if (input->KeyPress(SDLK_a)|| input->KeyPress(SDLK_LEFT)) {
      sprites[8]->Move(-93,0);
      currentUISelection--;
    }
    if (input->KeyPress(SDLK_l) || input->KeyPress(SDLK_RETURN)) {
      // Trigger Iron Maiden
      ironMaiden->Trigger();
    }
    break;
  case 2: // Guillotine
    if (input->KeyPress(SDLK_d)|| input->KeyPress(SDLK_RIGHT)) {
      sprites[8]->Move(93,0);
      currentUISelection++;
    }
    if (input->KeyPress(SDLK_a)|| input->KeyPress(SDLK_LEFT)) {
      sprites[8]->Move(-94,0);
      currentUISelection--;
    }
    if (input->KeyPress(SDLK_l) || input->KeyPress(SDLK_RETURN)) {				
      // Trigger Guillotine
      guillotine->Fall();
    }
    break;
  case 3:	// Logs	
    if (input->KeyPress(SDLK_d)|| input->KeyPress(SDLK_RIGHT)) {
      sprites[8]->Move(93,0);
      currentUISelection++;
    }
    if (input->KeyPress(SDLK_a)|| input->KeyPress(SDLK_LEFT)) {
      sprites[8]->Move(-93,0);
      currentUISelection--;
    }
    if (input->KeyPress(SDLK_l) || input->KeyPress(SDLK_RETURN) ) {
      // Trigger Logs
      logs->OpenDoor();
    }
    break;
  case 4:	// Pendulum
    if (input->KeyPress(SDLK_a)|| input->KeyPress(SDLK_LEFT)) {
      sprites[8]->Move(-93,0);
      currentUISelection--;
    }
    if (input->KeyPress(SDLK_l) || input->KeyPress(SDLK_RETURN) ) {
      // Trigger Pendulum
		pendulum->Trigger();
    }
    break;
  }

  // Tester for reloading scene
  /* if (input->KeyPress(SDLK_l)){
  manager->ChangeScene("Level1");*/

  UpdateCamera(playerVelocity);
  world->Update(_gameTime);

  if (ragdoll->playerPosition().x > 18.5 && trapsHit == 0)
    GameOver();
  else if(ragdoll->playerPosition().x > 18.5)
	  NextLevel();
}


void Level1Scene::Render(SpriteBatch& _spriteBatch) {
  for (int i = 0; i< SPRITENUMBER; i++)
    if (sprites[i] && camera->IsInView(sprites[i]->GetPosition(),sprites[i]->GetDimensions()))
      sprites[i]->Draw(_spriteBatch);
  //RenderUI(_spriteBatch);
  world->Draw(_spriteBatch);
}

//void Level1Scene::RenderUI(SpriteBatch& _spriteBatch) {
//	
//	Sprite *scoreDigit;
//	std::string scoreString = std::to_string(score);		
//	
//	ResourceManager* resources = ResourceManager::GetInstance(); // Get the resource manager for loading 
//	for (int i = 0; i< scoreString.length(); i++) {				
//		std::string file = "Textures/Backgrounds/" + std::to_string((int)scoreString[i] - '0') + ".png";
//		Sprite *scoreDigit = new Sprite(resources->Load<GLTexture>(file),glm::vec4(-250+i*25, -295, 25, 40));
//		scoreDigit->Draw(_spriteBatch);
//		delete scoreDigit;
//	}
//		
//  
//}

void Level1Scene::UpdateCamera(float xVelocity) {
  // moving background	
 // if(ragdoll->playerPosition().x < 15 && ragdoll->playerPosition().x > 0) {	
    camera->SetPosition(glm::vec2(camera->GetPosition().x + xVelocity, camera->GetPosition().y ));
    sprites[4]->Move(xVelocity, 0.0); // move UI layer
    sprites[8]->Move(xVelocity, 0.0); // move UI Selector

 // }
  /*if (camera->GetPosition().x >= 1600) {
    camera->SetPosition(glm::vec2(1600, camera->GetPosition().y ));
  }

  else if (camera->GetPosition().x < 0 ) {
    camera->SetPosition(glm::vec2(1, camera->GetPosition().y ));
  }*/

  camera->Update();
}

void Level1Scene::NextLevel() {
  currentLevel++;
  characterSpeed+= 0.005f;
  Load();
}

void Level1Scene::GameOver() {
  AudioManager::PlayMusic(winMusic,true);
  manager->ChangeScene("Credits");	
}

void MyContactListener::BeginContact(b2Contact* contact) {

  //get Fixture A
  void* bodyUserData1 = contact->GetFixtureA()->GetBody()->GetUserData();
  //get Fixture B
  void* bodyUserData2 = contact->GetFixtureB()->GetBody()->GetUserData();

  // Grounded

  //check if Fixture A was the player & Fixture B is an enemy
  //if((bodyUserData1 > 0 && (int)bodyUserData1 > 0) && (int)bodyUserData2 == -1)
  //{
	 // static_cast<RagdollCharacter*> (bodyUserData1)->Grounded(true);
	
  //}
  ////check if Fixture B was the player & Fixture A is an enemy
  //if((bodyUserData2 > 0 && (int)bodyUserData2 > 0) && (int)bodyUserData1 == -1)
  //{
  //  static_cast<RagdollCharacter*> (bodyUserData2)->Grounded(true);
	
  //}

   // Wall
  //check if Fixture A was the player & Fixture B is a Wall
  if((bodyUserData1 > 0 && (int)bodyUserData1 > 0) && (int)bodyUserData2 == -2)
  {
    static_cast<RagdollCharacter*> (bodyUserData1)->FlipSpeed();
  }
  //check if Fixture B was the player & Fixture A is a Wall
  if((bodyUserData2 > 0 && (int)bodyUserData2 > 0) && (int)bodyUserData1 == -2)
  {
    static_cast<RagdollCharacter*> (bodyUserData2)->FlipSpeed();
  }

  // Traps 
  //check if Fixture A was the player & Fixture B is an enemy
  if((bodyUserData1 > 0 && (int)bodyUserData1 > 0) && (int)bodyUserData2 < -2)
  {
    static_cast<RagdollCharacter*> (bodyUserData1)->startContact();
	level->trapsHit++;
	switch ((int)bodyUserData2) {
	case -3: // Pendulum 
		static_cast<RagdollCharacter*> (bodyUserData1)->GetBody(0)->ApplyForceToCenter(level->pendulum->ApplyForce(level->time),true);
		break;
	case -4: // Guillotine
		static_cast<RagdollCharacter*> (bodyUserData1)->GetBody(0)->ApplyForceToCenter(level->guillotine->ApplyForce(level->time),true);
		break;
	case -5: // Ball&Chain
		static_cast<RagdollCharacter*> (bodyUserData1)->GetBody(0)->ApplyForceToCenter(level->ballAndChain->ApplyForce(level->time),true);
		break;
	default:
		// NOTHING
		break;
	} 
  }
  //check if Fixture B was the player & Fixture A is an enemy
  if((bodyUserData2 > 0 && (int)bodyUserData2 > 0) && (int)bodyUserData1 < -2)
  {
    static_cast<RagdollCharacter*> (bodyUserData2)->startContact();
	level->trapsHit++;

	switch ((int)bodyUserData1) {
	case -3: // Pendulum 
		static_cast<RagdollCharacter*> (bodyUserData2)->GetBody(0)->ApplyForceToCenter(level->pendulum->ApplyForce(level->time),true);
		break;
	case -4: // Guillotine
		static_cast<RagdollCharacter*> (bodyUserData2)->GetBody(0)->ApplyForceToCenter(level->guillotine->ApplyForce(level->time),true);
		break;
	case -5: // Ball&Chain
		static_cast<RagdollCharacter*> (bodyUserData2)->GetBody(0)->ApplyForceToCenter(level->ballAndChain->ApplyForce(level->time),true);
		break;
	default:
		// NOTHING
		break;
	} 
  }


 

}

void MyContactListener::EndContact(b2Contact* contact) {
  //get Fixture A
  void* bodyUserData1 = contact->GetFixtureA()->GetBody()->GetUserData();
  //get Fixture B
  void* bodyUserData2 = contact->GetFixtureB()->GetBody()->GetUserData();
  
  // Ground
  //check if Fixture A was the player & Fixture B is an enemy
  //if((bodyUserData1 > 0 && (int)bodyUserData1 > 0) && (int)bodyUserData2 == -1)
  //{
	 // static_cast<RagdollCharacter*> (bodyUserData1)->Grounded(false);
	
  //}
  ////check if Fixture B was the player & Fixture A is an enemy
  //if((bodyUserData2 > 0 && (int)bodyUserData2 > 0) && (int)bodyUserData1 == -1)
  //{
  //  static_cast<RagdollCharacter*> (bodyUserData2)->Grounded(false);
	
  //}
  
  // Trap
  //check if Fixture A was the player & Fixture B is an enemy
  if((bodyUserData1 > 0 && (int)bodyUserData1 > 0) && (int)bodyUserData2 < -2)
  {
    static_cast<RagdollCharacter*> (bodyUserData1)->endContact();
  }
  //check if Fixture B was the player & Fixture A is an enemy
  if((bodyUserData2 > 0 && (int)bodyUserData2 > 0) && (int)bodyUserData1 < -2)
  {
    static_cast<RagdollCharacter*> (bodyUserData2)->endContact();
  }
}