#include <project/audioparticle.h>

AudioParticle::AudioParticle(std::string filePath)
{
	this->sound = new Sound(filePath);
	started = false;
}

AudioParticle::~AudioParticle()
{
	delete sound;
}

void AudioParticle::start()
{
	started = true;
	sound->play();
}

bool AudioParticle::destroyMe()
{
	bool destroy = false;
	if (started && !sound->isPlaying())
	{
		destroy = true;
	}
	return destroy;
}

