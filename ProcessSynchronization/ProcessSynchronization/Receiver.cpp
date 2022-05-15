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
	int temp;
	bool b =true;
	while(b)
	{
		std::cout << "To read message from binary file input 1" << std::endl;
		std::cout << "To finish work input 2" << std::endl;
		std::cin >> temp;
		if (temp != 1 && temp != 2) 
		{
			cout << "Wrong command number! Try again!";
			continue;
		}
		if (temp == 2) 
		{
			break;
		}
		sizeOfFile = 0;
		std::fstream fin, fout;
		while (sizeOfFile == 0)
		{
			WaitForSingleObject(hMutex, INFINITE);
			// find the size of binary file
			fin.open(binFileName, std::ios::binary | std::ios::in);
			fin.seekg(0, std::ios::end);
			sizeOfFile = fin.tellg();
			fin.close();
			ReleaseMutex(hMutex);
			// if binary file is empty, receiver will sleep for some seconds
			if (sizeOfFile == 0)
			{
				Sleep(1000);
			}
		}
		WaitForSingleObject(hMutex, INFINITE);
		fin.open(binFileName, std::ios::binary | std::ios::in);
		char message[20];
		// read message and print it
		fin.read(message, 20 * sizeof(char));
		std::cout << message << std::endl;
		// delete first message from the file
		fout.open("tmpfile", std::ios::binary | std::ios::out | std::ios::trunc);
		while (!fin.eof())
		{
			fin.read(message, 20 * sizeof(char));
			if (!fin.eof())
			{
				fout.write(message, 20 * sizeof(char));
			}
		}
		fin.close();
		fout.close();
		fin.open("tmpfile", std::ios::binary | std::ios::in);
		fout.open(binFileName, std::ios::binary | std::ios::out | std::ios::trunc);
		while (!fin.eof())
		{
			fin.read(message, 20 * sizeof(char));
			if (!fin.eof())
			{
				fout.write(message, 20 * sizeof(char));
			}
		}
		fin.close();
		fout.close();
		remove("tmpfile");
		ReleaseMutex(hMutex);
	}
	CloseHandle(hMutex);
	return 0;
}