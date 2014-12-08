#pragma once
#include "Sprite.h"
#include <unordered_map>
#include <string>

// because it's so fucking hard for you to use what we learned last semester to make animations

class Animation {
public:
	std::vector<GLTexture> cellList;
	int currentCell;

	void AddCell(GLTexture _cell);
	void LoopAll(float _seconds);
	void Loop(int _start, int _end, float _seconds);
	void Stop();
	void GotoFrame(int _number);
	void PlayAll(float _seconds);
	void Play(int _start, int _end, float _seconds);

	void Update(GameTime& gameTime);

	Animation() : looping(false), stopped(false), playing(false), timeShift(0.0F), totalTime(0.0F), start(0), end(0), currentCell(0) {}
private:
	bool looping, stopped, playing;
	float timeShift, totalTime;
	int start, end;
};

class AnimatedSprite : public Sprite {
public:
	std::unordered_map<std::string, Animation*> animations;
	Animation* currentAnimation;
	bool animating;

	void AddAnimation(std::string _name, Animation* _animation);
	void ChangeAnimation(std::string _animName, float _seconds, int _start = 0, int _end = 0);

	virtual void Update(GameTime& _gameTime);

	AnimatedSprite(glm::vec4 _bounds) : Sprite(GLTexture(), _bounds), animating(true) {}
private:
	void UpdateAndLoadAnimation(GameTime& _gameTime);
};