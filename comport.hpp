#include <windows.h>
#include <vector>

bool openComport(int comNumber);
//do this for closing:
//CloseHandle(comport);

std::vector<HANDLE> findNOpenArduino();
//returns vector pointer with HADLE-s - opened arduinos
//array can be empty

bool loadSpeedSettings (HANDLE *comport, bool is_1_on, bool is_2_on, bool is_1_reverse, bool is_2_reverse,
                   byte speed_1, byte speed_2);
//returns false if error
//returns true either

float* getRopeLength(HANDLE *comport);
// returns array length[2] from 2 rope detectors
// returns NULL if error
