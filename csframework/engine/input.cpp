#include <engine/input.h>
#include <iostream>

Input* Input::instance = NULL;

Input::Input()
{
    this->scrollup = false;
    this->scrolldown = false;
    this->mustq = false;
    for(unsigned int i = 0; i < 282; i++)
	{
        keys[i] =  false;
        keysdown[i] = false;
        keysup[i] = false;

        keyalrdyup[i] = true;
        keyalrdydown[i] = false;

        if(i < 20)
		{
                mousebutton[i] = false;
                mousebuttondown[i] = false;
                mousebuttonup[i] = false;

                mousebuttonalrdyup[i] = true;
                mousebuttonalrdydown[i] = false;
        }
    }

	lastPulledChar = "";
}

//Get the singleton instance. If it doesn't exist, create a new one and assign it.
Input* Input::getInstance() 
{
	if (instance == NULL) 
	{
		instance = new Input();
	}

	return instance;
}

Vector2 Input::getMousePosition()
{
    Vector2 mpos = Vector2(mousepos.x, mousepos.y);
	mpos -= Camera::getWindowSize() / 2.0f;
	return mpos;
}

void Input::quit()
{
	mustq = true;
}

Input::~Input()
{

}

void Input::update()
{
	lastPulledChar = "";
    this->scrollup = false;
    this->scrolldown = false;
    int x,y;
	SDL_GetMouseState(&x, &y);
    mousepos = Vector2(x,y);

    SDL_Event event;
    for(unsigned int i = 0; i < 282; i++)
	{
        if(keysup[i])
		{
            keysup[i] =  false;
        }
        if(keysdown[i])
		{
            keysdown[i] = false;
        }

        if(i < 20)
		{
            if(mousebuttonup[i])
			{
                mousebuttonup[i] = false;
            }
            if(mousebuttondown[i])
			{
                mousebuttondown[i] = false;
            }
        }
    }

    // Hanndle SDL events
    while( SDL_PollEvent( &event ) ) 
	{
		int scancode = event.key.keysym.scancode;
		int mouseButton = event.button.button;

        switch( event.type )
		{
        case SDL_KEYDOWN:
            keyalrdyup[scancode] = false;

            if(keyalrdydown[scancode] == false)
			{
                keysdown[scancode] = true;
                keyalrdydown[scancode] = true;
            }
            keys[scancode] = true;
			break;

        case SDL_KEYUP:
            keyalrdydown[scancode] = false;

            if(keyalrdyup[scancode] == false)
			{
                keysup[scancode] = true;
                keyalrdyup[scancode] = true;
            }
            keys[scancode] = false;
			break;
        case SDL_QUIT:
			mustq = true;
			break;

        case SDL_MOUSEBUTTONDOWN:
            mousebuttonalrdyup[mouseButton] = false;

            if(mousebuttonalrdydown[mouseButton] == false)
			{
                mousebuttondown[mouseButton] = true;
                mousebuttonalrdydown[mouseButton] = true;
            }
            mousebutton[mouseButton] = true;
		 break;

        case SDL_MOUSEBUTTONUP:
            mousebuttonalrdydown[mouseButton] = false;

            if(mousebuttonalrdyup[mouseButton] == false)
			{
                mousebuttonup[mouseButton] = true;
                mousebuttonalrdyup[mouseButton] = true;
            }
            mousebutton[mouseButton] = false;
			break;
        case SDL_MOUSEWHEEL:
            if (event.wheel.y < 0)
			{
                this->scrolldown = true;
            }else
			{
                this->scrollup = true;
            }
			break;
		case SDL_TEXTINPUT:
			lastPulledChar = event.text.text;
			break;
        }

	
    }
}

bool Input::getKey(int keyCode)
{
    if(keys[SDL_GetScancodeFromKey(keyCode)])
	{
        return true;
    }
    return false;
}

bool Input::getKeyDown(int keyCode)
{
    if(keysdown[SDL_GetScancodeFromKey(keyCode)])
	{
        return true;
    }
    return false;
}

bool Input::getKeyUp(int keyCode)
{
    if(keysup[SDL_GetScancodeFromKey(keyCode)])
	{
        return true;
    }
    return false;
}

bool Input::getMouseButton(int number)
{
    if(mousebutton[number])
	{
        return true;
    }
    return false;
}

bool Input::getMouseButtonDown(int number)
{
    if(mousebuttondown[number])
	{
        return true;
    }
    return false;
}

bool Input::getMouseButtonUp(int number)
{
    if(mousebuttonup[number])
	{
        return true;
    }
    return false;
}
