#include <iostream>
#include "draw.hpp"

#define SPEED_CONSTANT 0.00781273843    //256 / 32 767 = 0.00781273843

int UI::drawNHandle(allInfo *INFO)
{
    std::cout << "drawNHandle" << std::endl;
    //MessageBox(NULL,"11 string\0",NULL,MB_OK);
    system("PAUSE");
    if (this->init()) //return 1;
        std::cout << "12 string" << std::endl;
    MessageBox(NULL,"11 string\0",NULL,MB_OK);
    if (this->makeWindow()) return 2;
    std::cout << "14 string" << std::endl;
        ///joystick managment///
    int i, jsnum = SDL_NumJoysticks();
    if (jsnum<=0)
    {
        std::cout << "No joysticks plugged" << std::endl;
        return 1;
    }
    SDL_Joystick *joystick_main=NULL, *joystick_stepper=NULL, *joystick_temp=NULL;
    SDL_JoystickEventState(SDL_ENABLE);
    for (i=0; i<jsnum;i++)
    {
        joystick_temp = SDL_JoystickOpen(i);
        if (strcmp("Joystick Aviator for Playstation 3/PC",SDL_JoystickName(i)))
            joystick_main = joystick_temp;
        if (strcmp("Joystick USB Steering Wheel",SDL_JoystickName(i)))
            joystick_stepper = joystick_temp;
        printf("Joystick %s opened\n", SDL_JoystickName(1));
        printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joystick_stepper));
        printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joystick_stepper));
        printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joystick_stepper));
    }
    if (joystick_main==NULL || joystick_stepper==NULL)
    {
        std::cout << "Unknown joystick or less of them" << std::endl;
        system("PAUSE");
        exit(3);
    }

/*---------------------------------------------------------------------------*/

    ///*MAIN LOOP*///

    bool done = false;
    SDL_Event event;
    decart *tempPoint;

    while (!done)
    {
        // message processing loop
        while (SDL_PollEvent(&event))
        {
            // check for messages
        switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                {
                done = true;
                break;
                }
                // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    break;
                }
            ///Joystick events:
            //axis-es
            case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */
            {
                if (event.jaxis.which == SDL_JoystickIndex(joystick_main))
                {
                    if( event.jaxis.axis == 0)
                    {
                        //printf ("You moved joystick axis #0. Currect value %d\n", event.jaxis.value);
                        INFO->speed_x=SPEED_CONSTANT*event.jaxis.value;
                    }
                    if( event.jaxis.axis == 1)
                    {
                        //printf ("You moved joystick axis #1. Currect value %d\n", event.jaxis.value);
                        INFO->speed_y=SPEED_CONSTANT*event.jaxis.value;
                    }
                }
                if (event.jaxis.which == SDL_JoystickIndex(joystick_stepper))
                {
                    //printf("You moved stepers.Currect value %d\n", event.jaxis.value);
                    INFO->speed_z=SPEED_CONSTANT*event.jaxis.value;
                }
                break;
                ///end of axis processing
            }
            case SDL_JOYBUTTONDOWN:
            {
                //printf ("You pushed button #%d\n", event.jbutton.button);
                if (event.jbutton.button == 2)
                {
                    tempPoint = new(decart);
                    //tempPoint->isWall = false;
                    tempPoint->x = dstrect.x;
                    tempPoint->y = dstrect.y;
                    tempPoint->z =0;
                    INFO->goodPlace.push_back(tempPoint);
                }
                 if (event.jbutton.button == 3)
                {
                    tempPoint = new(decart);
                    //tempPoint->isWall = true;
                    tempPoint->x = dstrect.x;
                    tempPoint->y = dstrect.y;
                    tempPoint->z =0;
                    INFO->badPlace.push_back(tempPoint);
                }
                break;
                }
            break;
            }
        } // end of message processing

        /*
        if (!inGoodPlace(*dstrect))
        {
            printf("no? it's bad place!\n");
            //do_not_fly
        }
        else
        {
            dstrect
            //INFO->motor();
        }
        */
        INFO->motor();
        this->draw();

    } /// end main loop
    this->closeEverything();
    return 0;
}

int UI::init()
{
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }
    //std::ios_base::sync_with_stdio(0);
   /* freopen( "CON", "w", stdout );
    freopen( "CON", "r", stdin );
    freopen( "CON", "w", stderr );*/
    return 0;
}

int UI::makeWindow()
{
       ///* Window management *///
    // create a new window
    this->screen = SDL_SetVideoMode(640, 480, 16,SDL_HWSURFACE|SDL_DOUBLEBUF);
    if (!this->screen)
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }

    // load an image
    this->blue = SDL_LoadBMP("blue.bmp");
    this->green = SDL_LoadBMP("green.bmp");
    this->red = SDL_LoadBMP("red.bmp");
    if (!this->blue)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }

    // centre the bitmap on screen
    this->dstrect.x=(this->screen->w - this->blue->w) / 2;
    this->dstrect.y =(this->screen->h - this->blue->h) / 2;
    return 0;
}

void UI::draw()
{
    /// DRAWING STARTS HERE

        // clear screen
        SDL_FillRect(this->screen, 0, SDL_MapRGB(this->screen->format, 0, 0, 0));
        //SDL_Rect prect;     //point
        ///draw points
        /*int size=0;
        size=goodPlace.size();
        while (i<size)
        {
            prect.x = goodPlace[i]->x;;
            prect.y = goodPlace[i]->y;
            SDL_BlitSurface(this->green, 0, this->screen, &prect);
            i++;
        }
        size=badPlace.size();
        for(int i=0; i<size; i++)
        {
            prect.x = goodPlace[i]->x;;
            prect.y = goodPlace[i]->y;
            SDL_BlitSurface(red, 0, screen, &prect);
        }
        */

        // draw cursore
        SDL_BlitSurface(this->blue, 0, this->screen, &(this->dstrect));


    /// DRAWING ENDS HERE
        // finally, update the screen :)
        SDL_Flip(this->screen);
}

int UI::closeEverything()
{
    // free loaded bitmap
    SDL_FreeSurface(this->blue);
    SDL_Quit();
    std::cout << "SDL exited cleanly" << std::endl;
    return 0;
}
