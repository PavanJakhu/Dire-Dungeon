#pragma once

struct GameTime {
	// The time in seonds since the start of the game, at the start of the frame
	float time;
	// The time in seconds since the previous frame
	float deltaTime;
	// The time since the previous frame, unaffected by timeScale
	float unscaledDeltaTime;
	// The factor which controls how fast updating occurs. 1 = Normal, 0 = Paused, 0.5 = Half Speed, 2 = Double Speed
	float timeScale;
	// The number of frames since the start of the game
	int frameCount;

	GameTime() : time(0.0F), deltaTime(0.0F), unscaledDeltaTime(0.0F), timeScale(1.0F), frameCount(0) {}
};