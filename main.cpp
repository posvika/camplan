#include <iostream>
#include <atomic>
#include <vector>
#include <windows.h>

#include "comport.hpp"
#include "motor.hpp"

int main()
{
    allInfo *INFO=new(allInfo);
    INFO->arduino=findNOpenArduino();
        /*std::cout << "arduino detected " << INFO->arduino.size() << std::endl;
    for(unsigned int i=0;i<INFO->arduino.size();i++)
        std::cout << "ard: " << INFO->arduino[i] << std::endl;
    */
    INFO->initMotorBase();
    ///поток для подсчета текущего положения
    DWORD threadId;
    HANDLE thread_place = CreateThread(NULL,
                                       0,
                                       (LPTHREAD_START_ROUTINE)getCurrPlace,
                                       INFO,
                                       0,
                                       &threadId);
    std::cout << "Thread " << thread_place << " generated" << std::endl;
    system("PAUSE");
    return 0;
}
