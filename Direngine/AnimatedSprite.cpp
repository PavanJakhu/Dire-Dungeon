#include "AnimatedSprite.h"

void Animation::AddCell(GLTexture _cell) {
	cellList.push_back(_cell);
}

void Animation::LoopAll(float _seconds) {
	if (playing) return;

	stopped = false;
	if (looping) return;

	looping = true;
	start = 0;
	end = cellList.size() - 1;

	currentCell = start;
	//time for each cell to show
	timeShift = _seconds / (float)cellList.size(); 
}

void Animation::Loop(int _start, int _end, float _seconds) {
	if (playing) return;

	stopped = false;
	if (looping) return;

	looping = true;
	start = _start;
	end = _end;

	currentCell = _start;

	float difference = (float)_end - (float)_start;
	timeShift = _seconds / difference;
}

void Animation::Stop() {
	if (playing) return;

	stopped = true;
	looping = false;

	totalTime = 0.0f;
	timeShift = 0.0f;
}

void Animation::GotoFrame(int _number) {
	if (playing) return;

	if (_number < 0 || _number >= (signed)cellList.size()) return;

	currentCell = _number;
}

void Animation::PlayAll(float _seconds) {
	if (playing) return;

	GotoFrame(0);
	stopped = false;
	looping = false;
	playing = true;

	start = 0;
	end = (signed)cellList.size() - 1;

	timeShift = _seconds / (float)cellList.size(); 
}

void Animation::Play(int _start, int _end, float _seconds) {
	if (playing) return;

	GotoFrame(start);
	stopped = false;
	looping = false;
	playing = true;

	start = _start;
	end = _end;

	float difference = (float)_end - (float)_start;
	timeShift = _seconds / difference;
}

void Animation::Update(GameTime& gameTime) {
	if (stopped) return;
	// Don't change frame unless enough time has elapsed
	totalTime += (float)gameTime.time;
	if (totalTime > timeShift) {
		totalTime = 0.0f;
		//increment cell to draw
		currentCell++;
		//if looping and we're on the last frame, 
		//reset to first one again
		if (looping)
			if (currentCell > end) currentCell = start;
		//if we're at the end of the animation cycle, 
		//set booleans to indicate it
		if (currentCell > end) {
			currentCell = end;
			playing = false;
			stopped = true;
		}
	}
}

void AnimatedSprite::AddAnimation(std::string _name, Animation* _animation) {
	animations.insert(make_pair(_name, _animation));
}

void AnimatedSprite::ChangeAnimation(std::string _animName, float _seconds, int _start, int _end) {
	currentAnimation = animations[_animName];
	if (_end != -1 && _start != -1)
		currentAnimation->Play(_start, _start, _seconds);
	else
		currentAnimation->PlayAll(_seconds);
}

void AnimatedSprite::Update(GameTime& _gameTime) {
	UpdateAndLoadAnimation(_gameTime);
}

void AnimatedSprite::UpdateAndLoadAnimation(GameTime& _gameTime) {
	if (animating) {
		// Update the animation
		currentAnimation->Update(_gameTime);
		// Load the current image using the animation dictionary's CurrentAnimation name, and the CurrentCell number
		// Draw will draw this image next time around
		texture = currentAnimation->cellList[currentAnimation->currentCell];
	}
}