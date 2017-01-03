#pragma once

#include <string>
#include <engine/sound.h>

class AudioParticle
{
public:
	AudioParticle(std::string filePath);
	virtual ~AudioParticle();

	void start();
	bool destroyMe();
	Sound* getSound() { return sound; }
private:
	bool started;

	Sound* sound;
};

