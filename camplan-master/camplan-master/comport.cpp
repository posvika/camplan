#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <bitset>
#include <vector>

#include "comport.hpp"

using namespace std;

bool openComport(int comNumber)
//do this for closing:
//CloseHandle(comport);
{
    DCB comSettings;
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

std::vector<HANDLE> findNOpenArduino()
//returns vector pointer with HADLE-s - opened arduinos
//array can be empty
{
    std::vector<HANDLE > arduinos;
    DCB comSettings;
    int comNumber = -1, bStatus;
    char comName[10];

    HANDLE comport = NULL;
    for (int i=0;i<256;i++)
    {
        comNumber++;
        //cout << "next loop" << endl;
            sprintf(comName, "\\\\.\\COM%d", comNumber);
            comport = CreateFile(comName,             //open com3
                        GENERIC_READ | GENERIC_WRITE, // for reading and writing
                        0,                            // exclusive access
                        NULL,                         // no security attributes
                        OPEN_EXISTING,
                        0,
                        0);
            if (comport==INVALID_HANDLE_VALUE)
            {
                //cout << "Can't open port " << comNumber << endl;
                continue;
            }

        //cout << "comport"<<comNumber<< " opened" << endl;
        if (GetCommState(comport, &comSettings)==0)
            printf("GetCommState Error\n");
        else
        {
            ///DCB STRUCT
            comSettings.BaudRate = CBR_9600;     //CBR_9600 == 9600
            comSettings.StopBits = ONESTOPBIT;
            comSettings.ByteSize = 8;           //число информационных бит
            comSettings.Parity   = NOPARITY;    //нет бита четности
            comSettings.fParity  = FALSE;

            bStatus = SetCommState(comport, &comSettings);
            if (bStatus == 0)
                printf("SetCommState Error...\n");
            //очистить сом-порт от мусора после открытия:
            PurgeComm(comport,PURGE_TXABORT | PURGE_RXABORT );

            ///COMMTIMEOUTS STRUCT
            //seting up timeouts
            COMMTIMEOUTS time_out;
                time_out.ReadIntervalTimeout=0;
                time_out.ReadTotalTimeoutMultiplier=100;  //число бит*ReadTotalTimeoutMultiplier = таймоут на все чтение
                time_out.ReadTotalTimeoutConstant=100;    //все в миллисекундах
                time_out.WriteTotalTimeoutMultiplier=100;
                time_out.WriteTotalTimeoutConstant=100;

            SetCommTimeouts(comport,&time_out);

            BYTE tempByte;
            DWORD dwBytesTransferred;

                ///writing
                int buffer = 0xF5;
                DWORD written =0;
                if (!WriteFile(comport,&buffer,1,&written,NULL))
                    cout << "writing error" << endl;
                //cout << "0xF5 written" << endl;
                ///reading
                for (i=0; i<1; i++)
                {

                    ReadFile (comport, &tempByte, 1, &dwBytesTransferred, 0);
                    //cout << "read " << dwBytesTransferred << "bytes" << endl;
                    if (dwBytesTransferred == 1)
                    {
                        if (tempByte == 0xF6)
                        {
                            cout << "ok! it's arduino" << endl;
                            arduinos.push_back(comport);
                            system("PAUSE");
                            continue;   //do not close arduinos
                        }
                        else
                        {
                            //cout << "another port... " ;
                            continue;
                        }
                    }
                        //cout << tempByte << " ";
                        //cout << bitset<8>(tempByte) << " ";
                        //printf ("%c\t", (char)tempByte);
                }
                cout << endl;
        }
        CloseHandle(comport);
        //cout << "it's closed" << endl;
        //system ("PAUSE");
    }
    return arduinos;
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
    byte *_length = new byte[8];
    do
    {
        ReadFile (comport, _length, 8, &dwBytesTransferred, 0);
        ///могут быть отрицательными, все ок
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
