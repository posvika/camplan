//#include <SDL/SDL.h>

#include <cstdio>
#include <iostream>
#include <math.h>
#include <vector>
#include <windows.h>

#include "motor.hpp"

DWORD WINAPI getCurrPlace(LPVOID lpParameter)
{
    ///Функция выполняется отдельным потоком!!!
    allInfo *inf = (allInfo *)lpParameter;
    float *temp_radius1, *temp_radius2;
    /*************/
    /**MAIN LOOP**/
    /*************/
    while(1)
    {
        temp_radius1=getRopeLength(&(inf->arduino[0])); //взять длины первых 2ух тросов
        temp_radius2=getRopeLength(&(inf->arduino[1])); //взять длины вторых двух тросов
        if (!temp_radius1 | !temp_radius2)
            {
                std::cout << "getRopeLength error" << std::endl;
                continue; //ошибка чтения длин
            }
        //попытки захватить флаг
        while(true)
        {
            if(inf->isLocked.test_and_set(std::memory_order_acquire))
                Sleep(100); //ждать 1/10 секунды до следующей попытки
            break;  //удачно захватили
        }
            inf->frame[0].radius+=temp_radius1[0];
            inf->frame[1].radius+=temp_radius1[1];
            inf->frame[2].radius+=temp_radius1[0];
            inf->frame[3].radius+=temp_radius1[1];
        inf->isLocked.clear(); //отпустить флаг после записи
        for (int i=0;i<4;i++)
        {
            if (distance(&(inf->frame[i].base),&(inf->currPlace)) > inf->frame[i].radius)
                //try to change currPlace and check if it better
            {
                if (!inf->isLocked.test_and_set(std::memory_order_acquire))
                {
                inf->currPlace.x=(inf->currPlace.x < inf->frame[i].base.x)?inf->currPlace.x+=STEP:inf->currPlace.x-=STEP;
                inf->currPlace.y=(inf->currPlace.y < inf->frame[i].base.y)?inf->currPlace.y+=STEP:inf->currPlace.y-=STEP;
                inf->currPlace.z=(inf->currPlace.z < inf->frame[i].base.z)?inf->currPlace.z+=STEP:inf->currPlace.z-=STEP;
                inf->isLocked.clear();
                }
            }
        }

    }
    ExitThread(0);    // Специальный вызов для закрытия потока
}

/**
bool inGoodPlace(SDL_Rect *dstrect)
{
    doBadZones();
    doGoodZone();currPlace

}
**/

allInfo::allInfo()
{
    atomic_flag_clear(&isLocked);
    frame.resize(BASE_AMOUNT);
    for(int i=0;i<BASE_AMOUNT;i++)
        frame[i].dist.resize(BASE_AMOUNT-1);
    currPlace.x=0; currPlace.y=0; currPlace.z=0;
    speed_x=0; speed_y=0; speed_z=0;
}

void allInfo::initMotorBase()
{
    FILE *inFile=NULL;
    inFile = fopen("input.txt","r");
    int temp;
    std::vector<float > dist;   //вводимые расстояние ме
    for (int i=0;i<4;i++)
    {
        fscanf(inFile,"base%d|r=%fm|h=%fm|%fm|%fm|%fm\n",
               &temp,
               &(this->frame[i].radius),
               &(this->frame[i].height),
               &(this->frame[i].dist[0]),
               &(this->frame[i].dist[1]),
               &(this->frame[i].dist[2]));
        //assert -s - проверки на адекватность входных данных
    }
    ///I.Находим проекции расстояний на плоскость

    //ropeLengthDelta.x=0; ropeLengthDelta.y=0; ropeLengthDelta.z=0;
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
