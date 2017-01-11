#pragma once

#define MAX_MOUSE_BUTTONS 15
//defines max mouse buttons possible.

#include <string>

#include <SDL.h>

#include <engine/vector2.h>
#include <engine/camera.h>
class Input{
public:
    virtual ~Input();

    void update();
	void quit();

    bool getKey(int keyCode);
    bool getKeyUp(int keyCode);
    bool getKeyDown(int keyCode);

    bool getMouseButton(int number);
    bool getMouseButtonDown(int number);
    bool getMouseButtonUp(int number);

    Vector2 getMousePosition();

    bool mustQuit(){return mustq;};

    bool scrollUp(){return scrollup;}
    bool scrollDown(){return scrolldown;}

    int window_width;
    int window_height;

	static Input* getInstance();
	
	std::string getTextInput() { return lastPulledChar; }

private:
	Input();
	static Input* instance;

	std::string lastPulledChar;

    bool keys[SDL_NUM_SCANCODES];
    bool keysdown[SDL_NUM_SCANCODES];
    bool keysup[SDL_NUM_SCANCODES];
	
    bool keyalrdyup[SDL_NUM_SCANCODES];
    bool keyalrdydown[SDL_NUM_SCANCODES];
    bool mustq;

    bool mousebutton[MAX_MOUSE_BUTTONS];
    bool mousebuttondown[MAX_MOUSE_BUTTONS];
    bool mousebuttonup[MAX_MOUSE_BUTTONS];

    bool mousebuttonalrdydown[MAX_MOUSE_BUTTONS];
    bool mousebuttonalrdyup[MAX_MOUSE_BUTTONS];
	
    bool scrollup;
    bool scrolldown;

    Vector2 mousepos;
};