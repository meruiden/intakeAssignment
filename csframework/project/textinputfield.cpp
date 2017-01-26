#include <project/textinputfield.h>

TextInputField::TextInputField() : HudElement()
{
	backGround = new HudElement();
	backGround->addSprite("assets/images/whiteSquare.png");
	addChild(backGround);

	selected = false;
	selectable = true;


	inputText = new HudText();
	inputText->loadFont("assets/arial.ttf");
	inputText->setText("No Name");
	addChild(inputText);
	
	inputText->color = BLACK;
	setSelected(false);

	pipeDelay = 0.5f;
	pipeDelayCounter = 0;
	pipeVisable = false;
}


TextInputField::~TextInputField()
{
	removeChild(backGround);
	delete backGround;
	removeChild(inputText);
	delete inputText;
}

void TextInputField::update(float deltaTime)
{
	if (pipeDelayCounter >= pipeDelay)
	{

		if (pipeVisable)
		{
			std::string curText = inputText->getText();
			curText.pop_back();
			inputText->setText(curText);
		}
		else
		{
			std::string curText = inputText->getText();
			curText += "|";
			inputText->setText(curText);
		}
		pipeVisable = !pipeVisable;
		pipeDelayCounter = 0;

	}
	
	if (selected)
	{
		pipeDelayCounter += deltaTime;
	}
	

	if (beingHovered())
	{
		if (!selected && selectable)
		{
			inputText->color.a = 200;
			if (input()->getMouseButtonUp(1))
			{
				setSelected(true);
			}
		}
		
	}
	else
	{
		if (!selected && selectable)
		{
			inputText->color.a = 155;
		}
		if (input()->getMouseButtonUp(1))
		{
			setSelected(false);
		}
	}

	
	Vector2 inputOffset = Vector2(inputText->getSprite()->getSpriteSize().x / 2.0f, 0);
	backGround->setPosition(inputText->getPosition());
	backGround->setScale(Vector2(inputOffset.x * 2 / 32.0f + 0.5f, 1));
	inputText->setPosition(inputOffset);


	if (selected)
	{
		if (pipeVisable)
		{
			std::string curText = inputText->getText();
			curText.pop_back();
			inputText->setText(curText);
		}
		std::string curText = inputText->getText();
		inputText->pullTextInput();
		if (curText != inputText->getText())
		{
			pipeDelayCounter = 0;
		}
		if (pipeVisable)
		{
			std::string curText = inputText->getText();
			curText += "|";
			inputText->setText(curText);
		}
		if (input()->getKeyDown(SDLK_RETURN))
		{
			setSelected(false);
		}
	}
}

void TextInputField::setSelected(bool value)
{
	if (!selectable)
	{
		return;
	}

	selected = value;

	if (selected)
	{
		inputText->color.a = 255;
		pipeDelayCounter = 0;
	}
	else
	{
		if (pipeVisable)
		{
			std::string curText = inputText->getText();
			curText.pop_back();
			inputText->setText(curText);
			pipeVisable = false;
			pipeDelayCounter = 0;
		}
		inputText->color.a = 155;
	}
}

void TextInputField::setSelectAble(bool value)
{
	selectable = value;
}

void TextInputField::setText(std::string text)
{
	inputText->setText(text);
}

bool TextInputField::beingHovered()
{
	return backGround->overLapsWithPoint(input()->getMousePosition());
}

void TextInputField::setVisable(bool value)
{
	setSelected(false);
	setSelectAble(value);
	
	if (!value)
	{
		inputText->color.a = 0;
		backGround->color.a = 0;
	}
	else 
	{
		backGround->color.a = 255;
		setSelected(selected);
	}
}
