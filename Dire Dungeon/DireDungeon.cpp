#include "DireDungeon.h"

#include "Level1Scene.h"
#include "TitleScene.h"
#include "CreditScene.h"
#include "LoadingScene.h"

void DireDungeon::Init() {
  SetTitle("Dire Dungeon");
  SetDimensions(800, 640);
  SetFPSLimit(60.0F);
  Game::Init(); // SDL / OpenGL initialization, and Load() is called here

  camera.Init(screenWidth, screenHeight);

  shader.CompileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
  shader.AddAttribute("vertexPosition");
  shader.AddAttribute("vertexColor");
  shader.AddAttribute("vertexUV");
  shader.LinkShaders();

  spriteBatch.Init();
}

void DireDungeon::Load() {
  sceneManager = new SceneManager(this);
  sceneManager->AddScene("Loading", new LoadingScene(sceneManager));
  sceneManager->AddScene("Title", new TitleScene(sceneManager));
  sceneManager->AddScene("Level1", new Level1Scene(sceneManager));
  sceneManager->AddScene("Credits", new CreditScene(sceneManager));
  sceneManager->ChangeScene("Loading");	
}

void DireDungeon::Unload() {
  delete sceneManager;
//	delete test;
  Game::Unload();
}

void DireDungeon::Update(GameTime& _gameTime) {
  sceneManager->Update(_gameTime);
  bgColor = 0.5F*(sin(_gameTime.time) + 1);
}

void DireDungeon::Render() {
  glClearDepth(1.0); // Set depth to 1.0  
  glClearColor(bgColor,bgColor,bgColor,1.0); 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffer
  
  shader.Use(); // Enable the GLSL program, so that all drawing done is affected by the shader
  glActiveTexture(GL_TEXTURE0); // Use texture unit 0

  GLint textureLocation = shader.GetUniformLocation("textureSampler");
  glUniform1i(textureLocation, 0);
  GLint pLocation = shader.GetUniformLocation("P");
  glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(camera.GetCameraMatrix()[0][0]));

  spriteBatch.Begin();
  // Render the scene manager's current scene
  sceneManager->Render(spriteBatch);


  spriteBatch.End();

  glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
  shader.Unuse(); // Disable the shader
  window.SwapBuffer(); // Swap buffer and render
}