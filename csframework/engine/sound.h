#pragma once

#include <string>
#include <iostream>
#include <SDL_mixer.h>
#include <engine/config.h>
#include <engine/resourcemanager.h>

/// @brief Use this to play .wav audio files.
class Sound{
public:

	/// @brief Constructor.
	/// @param filepath The path to the audio file.
    Sound(std::string filepath);
	/// @brief Deconstructor. Also sets an spot free in availableChannels.
	virtual ~Sound();

	/// @brief Plays the loaded audio file.
	/// @param loop Set to true to loop the loaded audio until stop() is called.
    void play(bool loop);
	/// @brief Plays the loaded audio file without loop.
    void play();
	/// @brief Stops playing the audio file.
    void stop();
	/// @brief Set the volume of the loaded audio file.
	/// @param value Give an volume from 0 to 100.
	void setVolume(float value);

	/// @brief Set all audio channels available
	static void init();
private:
	/// @brief The SDL2 mixer audio chunk. This is where the loaded audio is being stored.
	Mix_Chunk* sound;

	/// @brief If there are more then MAX_AUDIO_PLAYING (in config.h) playing no channels are available. When there are no channels available, the audio cannot be played until another audio instance has been deleted. This function is called in the functions: Play, Stop and setVolume.
	void assignChannel();

	/// @brief The channel where the audio will be played on.
	unsigned int channel;
	/// @brief The volume of the audio.
	int volume;
	/// @brief If assignChannel() succeeded. this will be true.
	bool hasChannel;
	/// @brief Keeps track of the available channels.
	static bool availableChannels[MAX_AUDIO_PLAYING];
	
	
};
