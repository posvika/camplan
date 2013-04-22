/**************************************************/
/* For more information                           */
/* look here                                      */
/* http://citforum.ru/hardware/articles/comports/ */
/*                                                */
/* Эта программа открывает первые 256 сом-портов  */
/* и ищет там нужные ардуины. Это пример.         */
/**************************************************/
#define COM_NUMBER 4
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <bitset>

using namespace std;

int main (){

    DCB comSettings;
    int comNumber = -1, bStatus,i;
    char comName[10];

    HANDLE comport = NULL;
    for (int i=0;i<256;i++)
    {
        comNumber++;
        cout << "next loop" << endl;
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
                cout << "Can't open port " << comNumber << endl;
                continue;
            }

        cout << "comport"<<comNumber<< " opened" << endl;
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
                cout << "0xF5 written" << endl;
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
                            system("PAUSE");
                            exit(0);
                        }
                        else
                        {
                            cout << "another port... " ;
                            continue;
                        }
                    }
                        //cout << tempByte << " ";
                        //cout << bitset<8>(tempByte) << " ";
                        //printf ("%c\t", (char)tempByte);
                }
                cout << endl;
                //system("Pause");

        }
        CloseHandle(comport);
        cout << "it's closed" << endl;
        system ("PAUSE");
    }
}
