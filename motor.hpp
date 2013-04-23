//#include <SDL/SDL.h>

#include <windows.h>
#include <cstdio>
#include <vector>
#include <atomic>

#include "decart.hpp"
#include "comport.hpp"

#define BASE_AMOUNT 4   //число креплений
#define DELTA 0.1       //10sm
#define STEP 0.05       //5sm - аг корректировки расстояния getCurrPlace()

extern int speed_x, speed_y, speed_z;

struct base
{
    decart base;
    float radius;
    float height;
    std::vector<float > dist;   //расстояния до остальных камер(по часовой стрелке)
    std::vector<float > _flatDist; //проекции расстояний на плоскость
};

class allInfo   ///only one
{
    public:
    std::atomic_flag isLocked;          //for multithreading
    std::vector<HANDLE > arduino;       //2 ардуины управляют по 2 частотника каждая
    std::vector<base> frame;          //array of rope base coord.
    decart currPlace;                   //camera current place
    std::vector<decart *> goodPlace, badPlace; //zones points array

    //decart ropeLengthDelta;
    float MAX_X, MAX_Y, MAX_Z;
    byte speed_x, speed_y, speed_z;
    allInfo();
    void initMotorBase();
};

//void initMotorBase(allInfo *INFO);
DWORD WINAPI getCurrPlace(LPVOID lpParam);
void motor();
