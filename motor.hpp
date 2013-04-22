//#include <SDL/SDL.h>

#include <windows.h>
#include <cstdio>
#include <vector>
#include <atomic>

#include "decart.hpp"

#define DELTA 0.1 //10sm
#define STEP 0.05 //5sm

extern int speed_x, speed_y, speed_z;

class allInfo   ///only one
{
    public:
    std::atomic_flag isLocked;          //for multithreading
    std::vector<camera> frame;          //array of rope base coord.
    decart currPlace;                   //camera current place
    std::vector<decart *> goodPlace, badPlace; //zones points array
};

void initMotorBase();
DWORD WINAPI getCurrPlace(LPVOID lpParam);
void motor();
