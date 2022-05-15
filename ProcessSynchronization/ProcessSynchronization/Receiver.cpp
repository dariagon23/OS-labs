#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdio.h>

int main()
{
	char binFileName[100];
	int count; //the numer of records
	std::cout << "Input binary file name:" << std::endl;
	std::cin >> binFileName;
	std::cout << "Input the number of records in the binary file:" << std::endl;
	std::cin >> count;
	std::fstream file(binFileName,std::ios::binary|std::ios::out);
	if (!file.is_open()) {
		std::cout << "Error while creating file!" << std::endl;
		return 0;
	}
	file.close();
	int numberOfSenders;
	std::cout << "Input the number of Senders:" << std::endl;
	std::cin >> numberOfSenders;
	char commandLine[100];
	sprintf(commandLine, "%s %s %d", "Sender.exe", binFileName, count);
	// ready to work signal
	HANDLE hWorkEvent;
	CHAR EventName[] = "ReadyToWork";
	hWorkEvent = CreateEvent(NULL, FALSE, FALSE, EventName);
	// creation of mutex
	HANDLE	hMutex;
	hMutex = CreateMutex(NULL, FALSE, "Sender");
	
	STARTUPINFO *si = new STARTUPINFO[numberOfSenders];
	PROCESS_INFORMATION *piCom = new PROCESS_INFORMATION[numberOfSenders];
	// run numberOfSenders process
	for (int i = 0; i < numberOfSenders; i++)
	{
		ZeroMemory(&si[i], sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);
		// creation of console process
		CreateProcess(NULL, (LPTSTR)commandLine, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &si[i], &piCom[i]);
	}
	// waiting for ready to work signal
	for (int i = 0; i < numberOfSenders; i++)
	{
		WaitForSingleObject(hWorkEvent, INFINITE);
	}
	CloseHandle(hWorkEvent);

	return 0;
}