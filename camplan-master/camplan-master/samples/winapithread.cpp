#include <iostream>
#include <windows.h>

DWORD WINAPI getCurrPlace(LPVOID lpParameter)
{
    for(int i=0; i<5; ++i) {
        std::cout << "Thread  executing\n";
    }
    ExitThread(0);    // Специальный вызов для закрытия потока
}

int main()
{
    ///поток для подсчета текущего положения
    DWORD threadId;
    HANDLE thread_place = CreateThread(NULL,
                                       0,
                                       (LPTHREAD_START_ROUTINE)getCurrPlace,
                                       NULL,
                                       0,
                                       &threadId);
    system("PAUSE");
}
