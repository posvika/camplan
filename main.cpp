#include <iostream>
#include <atomic>
#include <vector>
#include <windows.h>

#include "comport.hpp"
#include "motor.hpp"

/*
DWORD WINAPI getCurrPlace(LPVOID lpParameter)
{
    for(int i=0; i<5; ++i) {
        std::cout << "Thread  executing\n";
    }
    ExitThread(0);    // Специальный вызов для закрытия потока
}
*/

int main()
{
    allInfo *INFO=new(allInfo);
    ///поток для подсчета текущего положения
    DWORD threadId;
    HANDLE thread_place = CreateThread(NULL,
                                       0,
                                       (LPTHREAD_START_ROUTINE)getCurrPlace,
                                       INFO,
                                       0,
                                       &threadId);

    openComport(COM_NUMBER);
    system("PAUSE");
    return 0;
}
