#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdio.h>


int main(int argc, char *argv[])
{
	HANDLE hWorkEvent;
	CHAR EventName[] = "ReadyToWork";
	hWorkEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, EventName);
	// send ready to work signal
	SetEvent(hWorkEvent);
	// binary file name from command line
	char binFileName[100];
	sprintf(binFileName, "%s", argv[1]);
	// number of records in binary file from command line
	char countRecords[10];
	sprintf(countRecords, "%s", argv[2]);
	int countRec = atoi(countRecords);
	int maxFileSize = 20 * countRec;
	std::fstream fout;
	HANDLE	hMutex;
	hMutex = OpenMutex(SYNCHRONIZE, FALSE, "Sender");
	int temp;
	bool b = true;
	int fileSize;
	char message[20];
	while (b)
	{
		std::cout << "To message to Receiver input 1" << std::endl;
		std::cout << "To finish work input 2" << std::endl;
		std::cin >> temp;
		if (temp != 1 && temp != 2) 
		{
			std::cout << "Wrong command number! Try again!" << std::endl;
			continue;
		}
		if (temp == 2) 
		{
			break;
		}
		std::cout << "Enter the message: " << std::endl;
		std::cin >> message;
		fileSize = maxFileSize;
		while (fileSize >= maxFileSize)
		{
			WaitForSingleObject(hMutex, INFINITE);
			// size of binary file
			fout.open(binFileName, std::ios::binary | std::ios::in);
			fout.seekg(0, std::ios::end);
			fileSize = fout.tellg();
			fout.close();
			ReleaseMutex(hMutex);
			// waiting for some seconds if file is not empty
			if (fileSize >= maxFileSize)
			{
				Sleep(1000);
			}
		}
		WaitForSingleObject(hMutex, INFINITE);
		// write message to the file
		fout.open(binFileName, std::ios::out | std::ios::app | std::ios::binary);
		fout.write(message, 20 * sizeof(char));
		fout.close();
		ReleaseMutex(hMutex);
	}
	return 0;
}