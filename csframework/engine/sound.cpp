#include <engine/sound.h>

bool Sound::availableChannels[MAX_AUDIO_PLAYING] = {};

Sound::Sound(std::string filepath){
	// Setting values.
	hasChannel = false;
	channel = -1;
	volume = 100;
	
	bool loadingSucces = false;
	sound = ResourceManager::getInstance()->getLoadedWav(filepath, loadingSucces); // Load the sound from ResourceManager.

	// Check if loading the sound succeeded.
	if (loadingSucces) {
		assignChannel(); // Assign a free channel to this sound.
	}
}

void Sound::assignChannel() 
{

	// Loop trough all available channels.
	for (unsigned int i = 0; i < MAX_AUDIO_PLAYING; i++) {
		if (Sound::availableChannels[i]) {
			this->channel = i;
			hasChannel = true;
			break; // If an available channel has been found. Assign it and break out of the loop.
		}
	}
}

void Sound::init()
{
	// Init all channels to be available.
	for (unsigned int i = 0; i < MAX_AUDIO_PLAYING; i++) {
		Sound::availableChannels[i] = true;
	}
}
Sound::~Sound(){
	if (hasChannel) { // If this instance is deleted and a channel whas assigned to this sound. set that channel available again.
		Sound::availableChannels[this->channel] = true;
	}
}

void Sound::play(){
	// Play the sound without loop.
    this->play(false);
}

void Sound::play(bool loop){

	if (sound == NULL) {
		return; // If there is no sound loaded. return.
	}

	// If no channel had been assigned yet. Try assign one.
	if (!hasChannel) { 
		assignChannel();
		if (!hasChannel) {
			return; // If no channel has been assigned yet. return.
		}
	}

    int loopornot = 0;

    if(loop){ // Check if the audio has to loop
        loopornot = -1; // If true, set to -1. Mix_PlayChannel uses -1 for loop and 0 to play it just once. 
    }else{
        loopornot = 0;
    }
    Mix_PlayChannel(this->channel, sound, loopornot); // Play the sound
}

void Sound::setVolume(float value){
	// If no channel had been assigned yet. Try assign one.
	if (!hasChannel) {
		assignChannel();
		if (!hasChannel) {
			return; // If no channel has been assigned yet. return.
		}
	}

	float v = 128.0f / 100; // SDL2 Mixer uses an volume range of 0 to 128. But i want my framework to use an range of 0 to 100. So i map that value.
    v *= value;
    this->volume = value; // Set the volume variable of this instance
    Mix_Volume(this->channel, (int)v); // Set the volume

}

void Sound::stop(){
	// If no channel had been assigned yet. Try assign one.
	if (!hasChannel) {
		assignChannel();
		if (!hasChannel) {
			return; // If no channel has been assigned yet. return.
		}
	}

    Mix_HaltChannel(this->channel); // Stop playing the audio.
}
