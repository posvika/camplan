//#include <SDL/SDL.h>

#include <cstdio>
#include <math.h>
#include <vector>
#include <windows.h>

#include "motor.hpp"

///decart place constants
decart ropeLengthDelta;
double MAX_X, MAX_Y, MAX_Z;
int speed_x=0, speed_y=0, speed_z=0;
std::vector<camera> frame;


DWORD WINAPI getCurrPlace(LPVOID lpParameter)
{
    ///Функция выполняется отдельным потоком!!!
    printf("Something done by place thread\n");
    allInfo *inf = (allInfo *)lpParameter;
    int i;
    /*
    while(1)
    {
        for (i=0;i<4;i++)
        {
            if (distance(&frame[i].base,&(inf->currPlace)) > frame[i].radius)
                //try to change currPlace and check if it better
            {
                if (!inf->isLocked.test_and_set(std::memory_order_acquire))
                {
                inf->currPlace.x=(inf->currPlace.x < frame[i].base.x)?inf->currPlace.x+=STEP:inf->currPlace.x-=STEP;
                inf->currPlace.y=(inf->currPlace.y < frame[i].base.y)?inf->currPlace.y+=STEP:inf->currPlace.y-=STEP;
                inf->currPlace.z=(inf->currPlace.z < frame[i].base.z)?inf->currPlace.z+=STEP:inf->currPlace.z-=STEP;
                inf->isLocked.clear();
                }
            }
        }

    }
    */
    ExitThread(0);    // Специальный вызов для закрытия потока
}

/**
bool inGoodPlace(SDL_Rect *dstrect)
{
    doBadZones();
    doGoodZone();currPlace

}
**/

void initMotorBase()
{
    /*
    frame.resize(4);
    int i;
    FILE *inFile=NULL;
    inFile = fopen("input.txt","r");

    MAX_X=0, MAX_Y=0, MAX_Z =0;
    for (i=0;i<4;i++)
    {
        fscanf(inFile,"cam|r=%f|x=%lf|y=%lf|z=%lf\n",&frame[i].radius,&frame[i].base.x,&frame[i].base.y,&frame[i].base.z);
        MAX_X=(frame[i].base.x > MAX_X)?frame[i].base.x:MAX_X;
        MAX_Y=(frame[i].base.y > MAX_Y)?frame[i].base.y:MAX_Y;
        MAX_Z=(frame[i].base.z > MAX_Z)?frame[i].base.z:MAX_Z;
        //assert -s
    }
    INFO.currPlace.x=MAX_X/2; INFO.currPlace.y=MAX_Y/2; INFO.currPlace.z=MAX_Z/2;
    ropeLengthDelta.x=0; ropeLengthDelta.y=0; ropeLengthDelta.z=0;
    //getCurrPlace();
    */
}

void motor()
{
    /*
    ///additional info in frec_format.txt
    byte i=0;
    decart *temp;
    std::vector<decart *> axis;
    decart speed;
    speed.x=speed_x;
    speed.y=speed_y;
    speed.z=speed_z;
    ///array for arduino mega
    byte outInfo[8];
    outInfo[0] = 0;     //it's for mega
    outInfo[1] = 5;     //it's from desktop
    outInfo[2] = 11;    //frec. command
    outInfo[3] = 15;    //turn on all frec-s
    std::vector<int > spd;
    spd.resize(4);
    for (i=0;i<4;i++)
    {
        temp=new(decart);
        temp=do_vector_from_null(&INFO.currPlace,&frame[i].base);
        axis.push_back(temp);
        spd[i]=proect(&speed,axis[i]);
        outInfo[3]=spd[i]?((outInfo[3])&(~1<<i)):((outInfo[3])|(1<<i));
        outInfo[4+i]=abs(spd[i]);
    }
    //отправка скоростей моторам, открытие com порта
    int comNumber = COM_NUMBER;
    char comName[10];
    sprintf(comName, "\\\\.\\COM%d", comNumber);
    HANDLE comport = NULL;
    do
    {
        comport = CreateFile(comName,             //open com3
                    GENERIC_READ | GENERIC_WRITE, // for reading and writing
                    0,                            // exclusive access
                    NULL,                         // no security attributes
                    OPEN_EXISTING,
                    0,
                    0);
    }
    while (comport==INVALID_HANDLE_VALUE);  //try to open again, if comport open fails
        */
}
