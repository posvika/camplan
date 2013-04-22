#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <bitset>

#include "comport.hpp"

using namespace std;

bool openComport(int comNumber)
//do this for closing
//CloseHandle(comport);
{
    DCB comSettings;
    //int comNumber = COM_NUMBER,
    int bStatus;
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
    cout << "comport opened" << endl;
    if (GetCommState(comport, &comSettings)==0)
		printf("GetCommState Error\n");
	else{
        do{
            comSettings.BaudRate = 9600;
            comSettings.StopBits = ONESTOPBIT;
            comSettings.ByteSize = 8;
            comSettings.Parity   = NOPARITY;
            comSettings.fParity  = FALSE;

            bStatus = SetCommState(comport, &comSettings);}
        while (!bStatus);
    }
    //очистить сом-порт от мусора после открытия:
    PurgeComm(comport,PURGE_TXABORT | PURGE_RXABORT );
    return true;
}

bool loadSpeedSettings (HANDLE *comport, bool is_1_on, bool is_2_on, bool is_1_reverse, bool is_2_reverse,
                   byte speed_1, byte speed_2)
//returns false if error
//returns true either
{
    //writing
    byte buffer[4];
    buffer[0] = 0xF4;
    buffer[1] = 0;
    if (is_1_on)        buffer[1]|1;
    if (is_2_on)        buffer[1]|1<<1;
    if (is_1_reverse)   buffer[1]|1<<2;
    if (is_2_reverse)   buffer[2]|1<<3;
    buffer[2]=speed_1; buffer[3]=speed_2;
    DWORD written=0;
    while (written!=4)
    {   if (!WriteFile(*comport,&buffer,4,&written,NULL))
        {
            cout << "writing error" << endl;
            return false;
        }
    }
    return true;
}

float* getRopeLength(HANDLE *comport)
// returns array length[2] from 2 rope detectors
// returns NULL if error
{
    //BYTE tempByte;
    DWORD dwBytesTransferred;
    //writing
    byte i, buffer = 0xD0;
    DWORD written =0;
    if (!WriteFile(*comport,&buffer,1,&written,NULL))
    cout << "writing error" << endl;
    cout << "0xDO written" << endl;
    //reading
    byte _length[8];
    do
    {
        ReadFile (comport, _length, 8, &dwBytesTransferred, 0);
        //cout << "read " << dwBytesTransferred << "bytes" << endl;
        /*if (dwBytesTransferred == 1)
        {
            _length[i]=tempByte;
            cout << tempByte << " ";
            //cout << bitset<8>(tempByte) << " ";
            //printf ("%c\t", (char)tempByte);
        }*/
    } while(dwBytesTransferred!=8);
    cout << endl;
    float *length = (float *)_length;
    for (i=0; i<2; i++)
        if (length[i] == -1) return NULL;
    return length;
}
