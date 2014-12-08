#include "Game.h"
#include "Errors.h"

void Game::Init() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    Debug::FatalError("Failed to initialize SDL: " + std::string(SDL_GetError()));
  // Initialize SDL_mixer
  if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    Debug::FatalError("Failed to initialized SDL_mixer: " + std::string(Mix_GetError()));

  // Tell SDL that we want a double buffered window so we dont get any flickering
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  // Create the window with the specified game title, dimensions, and flags
  window.Create(gameTitle, screenWidth, screenHeight, 0);

  state = GameState::PLAY;
  input = InputManager::GetInstance();
  resources = ResourceManager::GetInstance();

  Load();
}

void Game::Run() {
  Init();

	GameTime gameTime;
	float startTime = (int)SDL_GetTicks() / 1000.0F;

  int frameTicks = 0, lastTicks = 0;

  // The Game Loop
  while (state != GameState::EXIT) {
    lastTicks = frameTicks;
    frameTicks = (int)SDL_GetTicks();
    float delta = (float)(frameTicks - lastTicks);

		gameTime.time = (frameTicks / 1000.0F) - startTime;
		gameTime.unscaledDeltaTime = delta / 1000.0F;
		gameTime.deltaTime = gameTime.unscaledDeltaTime * gameTime.timeScale;

    SDL_Event e;
    if (input->Update(e) != 0) // InputManager::Update returns 1 if SDL_QUIT event is sent
      Exit();

    Update(gameTime);
    Render();

		gameTime.frameCount++;
    float endTicks = (float)SDL_GetTicks() - frameTicks;
    if (1000.0F / maxFPS > endTicks)
      SDL_Delay((Uint32)((1000.0F / maxFPS) - endTicks));
  }

  Quit();
}

void Game::Unload() {
  input->DestroyInputManager(input);
  resources->DestroyResourceManager(resources);
}

void Game::Quit() {
  Unload();
  Mix_Quit();
  SDL_Quit();
}