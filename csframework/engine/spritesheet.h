#pragma once

#include <string>

#include <engine/sprite.h>
#include <engine/resourcemanager.h>

/// @brief Used to load spritesheets for animations or using particular sprites in a spritesheets.  
class SpriteSheet
{
public:
	/// @brief Constuctor.
	SpriteSheet();

	/// @brief Deconstructor.
	~SpriteSheet();

	/// @brief Return the sprite.
	Sprite* getSprite() { return sprite; }
	
	/// @brief Load the spritesheet to an sprite.
	/// @param path The path where the image file is located. 
	/// @param spriteWidth Set to the width of one sprite in your spritesheet.
	/// @param spriteHeight Set to the height of one sprite in your spritesheet.
	/// @param spritesPerRow Set the number of sprites in one single row.
	/// @param rows Set the number of rows.
	void setUpSpriteSheet(std::string path, int spriteWidth, int spriteHeight, int spritesPerRow, int rows);
	/// @brief Set the frame to display an specific sprite from the spritesheet
	/// @param spriteId The place of the sprite in the spritesheet. 0 is the first sprite. Start top left.
	void setSprite(int spriteId);
	/// @brief Use to swap between the frames set with setAnimateFrames(int startFrame, int endFrame).
	/// @param deltaTime Pass the deltaTime given in the update function
	/// @param step The step in wich the sprites must swap. For example: 30 frames per second = 1.0f / 30.0f
	void animate(float deltaTime, float step);
	/// @brief Play the animation (if animate(float deltaTime, float step) is being called.)
	void play();
	/// @brief Stop playing animation (if animate(float deltaTime, float step) is being called.)
	void stop();
	/// @brief Pause playing the animation (if animate(float deltaTime, float step) is being called.)
	void pause(bool value);
	/// @brief Set the animation to loop. If true: the animation will play while animate(float deltaTime, float step) is being called. unless stopped or paused. if false: The animation will play untill the last frame and stop there until play() is called.
	void setLoop(bool value);
	/// @brief Set the frames in which the animation will be lerped.
	void setAnimateFrames(int startFrame, int endFrame);

	/// @brief Use to check if loop is set true or false.
	bool isLooping() { return loop; }
	/// @brief Use to check the animation is paused.
	bool isPaused() { return paused; }
	/// @brief Use to check the animation is playing.
	bool isPlaying() { return playing; }
	/// @brief Get the start frame. This is used by animate(float deltaTime, float step) as start frame.
	int getAnimationStartFrame() { return startFrame; }
	/// @brief Get the start frame. This is used by animate(float deltaTime, float step) as last frame.
	int getAnimationEndFrame() { return endFrame; }
	/// @brief Get the id of the current selected sprite set with setSprite(int spriteId);.
	int getCurSpriteId() { return curSpriteId; }

private:
	/// @brief The sprite of this spritesheet.
	Sprite* sprite;
	/// @brief The start frame of the animation.
	int startFrame;
	/// @brief The end frame of the animation.
	int endFrame;
	/// @brief The current frame of the animation.
	int curFrame;
	/// @brief The amount of sprites in one row.
	int spritesPerRow;
	/// @brief The amount of rows.
	int rows;
	/// @brief The active sprite.
	int curSpriteId;
	/// @brief The counter that adds deltaTime everyframe until animateCounter >= step.
	float animateCounter;
	/// @brief True if the animation is playing.
	bool playing;
	/// @brief True if the animation is paused.
	bool paused;
	/// @brief True if the animation must loop.
	bool loop;
};

