//#include <SDL/SDL.h>

#include <windows.h>
#include <cstdio>
#include <vector>
#include <atomic>

#include "decart.hpp"
#include "comport.hpp"

#define BASE_AMOUNT 4   //����� ���������
#define DELTA 0.1       //10sm
#define STEP 0.05       //5sm - �� ������������� ���������� getCurrPlace()

extern int speed_x, speed_y, speed_z;

struct base
{
    decart base;
    float radius;
    float height;
    std::vector<float > dist;   //���������� �� ��������� �����(�� ������� �������)
    std::vector<float > _flatDist; //�������� ���������� �� ���������
};

class allInfo   ///only one
{
    public:
    std::atomic_flag isLocked;          //for multithreading
    std::vector<HANDLE > arduino;       //2 ������� ��������� �� 2 ���������� ������
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
