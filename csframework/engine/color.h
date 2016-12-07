#ifndef COLOR_H
#define COLOR_H

#define BLACK Color(0,0,0)
#define WHITE Color(255,255,255)

#define LIGHT_BLUE Color(0,186,255)
#define BLUE Color(0,89,252)
#define DARK_BLUE Color(0,0,255)

#define LIGHT_GREEN Color(36,252,12)
#define GREEN Color(22,222,0)
#define DARK_GREEN Color(14,138,0)

#define LIGHT_YELLOW Color(255,255,23)
#define YELLOW Color(245,245,0)
#define DARK_YELLOW Color(213,213,0)

#define LIGHT_RED Color(255,16,16)
#define RED Color(222,0,0)
#define DARK_RED Color(190,0,0)

#define GREY Color(190,190,190)
#define DARK_GREY Color(104,104,104)

#define ORANGE Color(255,108,0)
#define PURPLE Color(166,0,202)
#define PINK Color(255,68,231)
#define CYAN Color(40,247,255)



class Color
{
public:
    Color(int _r = 255, int _g = 255, int _b = 255, float _a = 255);
    int r;
    int g;
    int b;
    float a;

private:

};
#endif
