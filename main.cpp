#include <iostream>
#include <atomic>
#include <vector>
#include <windows.h>

#include <SDL/SDL.h>
#include "comport.hpp"
#include "draw.hpp"

int main(int argc, char **argv)
{
    std::cout << "First instruction " << std::endl;
    /*freopen( "CON", "w", stdout );
    freopen( "CON", "r", stdin );
    freopen( "CON", "w", stderr );*/
    allInfo *INFO=new(allInfo);
    UI *pUI=new(UI);
    atexit((void (*)())pUI->closeEverything());
    std::cout << "before find arduino" << std::endl;
    INFO->arduino=findNOpenArduino();
        /*std::cout << "arduino detected " << INFO->arduino.size() << std::endl;
    for(unsigned int i=0;i<INFO->arduino.size();i++)
        std::cout << "ard: " << INFO->arduino[i] << std::endl;
    */
    std::cout << "before initMotorBase" << std::endl;
    INFO->initMotorBase();
    std::cout << "Before thread" << std::endl;
    ///поток для подсчета текущего положения
    DWORD threadId;
    HANDLE thread_place = CreateThread(NULL,
                                       0,
                                       (LPTHREAD_START_ROUTINE)getCurrPlace,
                                       INFO,
                                       0,
                                       &threadId);
    std::cout << "Thread " << thread_place << " generated" << std::endl;
    pUI->drawNHandle(INFO);
    system("PAUSE");
    return 0;
}
