#include <engine/spritesheet.h>


SpriteSheet::SpriteSheet()
{
	// Init all variables
	animateCounter = 0;
	startFrame = 0;
	endFrame = 0;
	spritesPerRow = 0;
	rows = 0;
	curSpriteId = 0;
	loop = false;
	sprite = new Sprite();
}

void SpriteSheet::setUpSpriteSheet(std::string path, int spriteWidth, int spriteHeight, int spritesPerRow, int rows)
{

	sprite->setUpSprite(path);
	sprite->setSpriteSize(Vector2(spriteWidth, spriteHeight));

	Vector2 uvSize = Vector2();
	uvSize.x = 1.0f / (float)spritesPerRow;
	uvSize.y = 1.0f / (float)rows;

	sprite->setUvSize(uvSize);

	this->spritesPerRow = spritesPerRow;
	this->rows = rows;
	
}

void SpriteSheet::setSprite(int spriteId)
{
	if (spriteId == curSpriteId) {
		return;
	}
	Vector2 offset = Vector2();
	if (spriteId <= 0) 
	{
		curSpriteId = 0;
		sprite->setUvOffset(offset);
		return;
	}

	int totalSprites = spritesPerRow * rows;
	if (spriteId > totalSprites) {
		spriteId = totalSprites;
	}

	int row = floor((float)spriteId / (float)spritesPerRow) ;
	int whereInRow = (spriteId % spritesPerRow) ;
	offset = Vector2(whereInRow * (1.0f / (float)spritesPerRow), row * (1.0f / (float)rows));

	sprite->setUvOffset(offset);
	curSpriteId = spriteId;
	curFrame = spriteId;
}

void SpriteSheet::animate(float deltaTime, float step)
{
	
	if (playing && !paused) {
		animateCounter += deltaTime;
		if (animateCounter >= step)
		{
			animateCounter = fmod(animateCounter, step);
			curFrame++;
			if (curFrame > endFrame)
			{
				curFrame = startFrame;
			}
			if (curSpriteId != curFrame)
			{
				setSprite(curFrame);
			}

		}
		if (!loop) 
		{
			if (curFrame == endFrame)
			{
				stop();
			}
		}
	}
}

void SpriteSheet::play()
{
	stop();
	playing = true;
	paused = false;
	animateCounter = 0.0f;
	setSprite(startFrame);
}

void SpriteSheet::stop()
{
	playing = false;
	paused = false;
	setSprite(endFrame);

}

void SpriteSheet::pause(bool value) 
{
	if (value)
	{
		paused = true;
	}
	else {
		paused = false;
		animateCounter = 0;
	}

}

void SpriteSheet::setLoop(bool value)
{
	loop = value;
}
void SpriteSheet::setAnimateFrames(int startFrame, int endFrame)
{
	this->startFrame = startFrame;
	this->endFrame = endFrame;
	curFrame = startFrame;
//	setSprite(startFrame);
}

SpriteSheet::~SpriteSheet()
{

}
