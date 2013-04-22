#include <windows.h>
#define COM_NUMBER 4

bool openComport(int comNumber = COM_NUMBER);
//do this for closing:
//CloseHandle(comport);

bool loadSpeedSettings (HANDLE *comport, bool is_1_on, bool is_2_on, bool is_1_reverse, bool is_2_reverse,
                   byte speed_1, byte speed_2);
//returns false if error
//returns true either

float* getRopeLength(HANDLE *comport);
// returns array length[2] from 2 rope detectors
// returns NULL if error
