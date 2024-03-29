//#include <SDL/SDL.h>

#include <windows.h>
#include <cstdio>
#include <vector>
#include <atomic>

#include "decart.hpp"
#include "comport.hpp"

#define BASE_AMOUNT 4   //����� ���������
#define ZONE_BOURDER 0.5 //50sm - ������� ����, ��� �������������� ��������
#define ZONE_HEIGHT  0.3 //30sm - ������ ������ - ������ ���� �������� ���
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
    ///�������� ��� ����������
    byte speedProecton;
    bool isReverse;
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
    ///motor() ������ ������� ���� ���, � ������ ������ ������ ������� � false:
    bool motor();
};

DWORD WINAPI getCurrPlace(LPVOID lpParam);
