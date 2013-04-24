#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL/SDL.h>
#endif

#include "motor.hpp"

class UI
{
public:
    allInfo *INFO;
    ///painting constants
    SDL_Surface* screen = NULL;
    SDL_Surface* blue = NULL, *green = NULL, *red = NULL;
    SDL_Rect dstrect;   //cursore

    int drawNHandle(allInfo *INFO);        ///MAIN FUNCTION
    int closeEverything();
private:
    int init();
    int makeWindow();
    void draw();
};
