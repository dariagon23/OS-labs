#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <Windows.h>
#include <stdio.h>
int arrSize;
int*arr;
HANDLE *hWorkEvent, *hStopEvent, *hFailEvent;
DWORD WINAPI marker(LPVOID _number)
{
	int number = (int)_number;
	HANDLE hControlEvent[3];
	hControlEvent[0] = hStopEvent[number - 1];			
	hControlEvent[1] = hWorkEvent[number - 1];			
	hControlEvent[2] = hFailEvent[number - 1];			
	//start working after signal from main
	WaitForSingleObject(hControlEvent[1], INFINITE);	
	srand(number);
	int temp = 0;
	bool bb = true;
	DWORD dwWaitSignal;
	while (bb)
	{
		ResetEvent(hControlEvent[1]);
		int random = rand();
		int c = random % arrSize;
		if (arr[c] == 0)
		{
			Sleep(5);
			arr[c] = number;
			Sleep(5);
			temp++;
		}
		else
		{
			std::cout << "Thread number: "<< number;
			std::cout << std::endl;
			std::cout << "The number of marked elements: "<< temp;
			std::cout << std::endl;
			std::cout << "Can't mark the element with number: "<< c;
			std::cout << std::endl;
			//signal to main 
			SetEvent(hControlEvent[2]);
			//waiting for signal from main.
			dwWaitSignal = WaitForMultipleObjects(2, hControlEvent, FALSE, INFINITE);
			if (dwWaitSignal == WAIT_OBJECT_0)
			{
				for (int i = 0; i < arrSize; i++)
				{
					if (arr[i] == number)
						arr[i] = 0;
				}
				temp = 0;
				bb = false;
				break;
			}
			else
			{
				ResetEvent(hControlEvent[2]);
			}
		}
	}
	return 0;
}
int main()
{
	//creating array
	std::cout << "Enter the number of elements of the array:" << std::endl;
	std::cin >> arrSize;
	arr=new int[arrSize];
	for(int i=0; i<arrSize;i++)
		arr[i]=0;
	//creating threads
	std::cout << "Enter the number of marker threads:" << std::endl;
	int numOfMarkerThreads;
	std::cin>>numOfMarkerThreads;
	// init of critical section
	CRITICAL_SECTION cs;
	InitializeCriticalSection(&cs);
	//start work with marker
	HANDLE *hMarker = new HANDLE[numOfMarkerThreads];
	DWORD *dwMarker = new DWORD[numOfMarkerThreads];
	//storage of threads' numbers in array, numeration from 1
	int *numbers = new int[numOfMarkerThreads];
	//thread starts working
	hWorkEvent = new HANDLE[numOfMarkerThreads];
	//thread stops working
	hStopEvent = new HANDLE[numOfMarkerThreads];
	//thread can't work any more
	hFailEvent = new HANDLE[numOfMarkerThreads];
	for (int i = 0; i < numOfMarkerThreads; i++)
	{
		hWorkEvent[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		hStopEvent[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		hFailEvent[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		//numbers of the threads
		numbers[i] = i + 1;
		// run numOfMarkerThreads marker threads
		hMarker[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)marker,
			(LPVOID) numbers[i], 0, &dwMarker[i]);
		if (NULL == hMarker[i])
			return GetLastError();
	}
	
	bool check = true;
	while (check)
	{
		EnterCriticalSection(&cs); 		
		for (int i = 0; i < numOfMarkerThreads; i++)
		{
			ResetEvent(hFailEvent[i]);
			//signal for threads to start working
			SetEvent(hWorkEvent[i]);
		}
		//waiting for all threads can't work any more
		WaitForMultipleObjects(numOfMarkerThreads, hFailEvent, TRUE, INFINITE);
		LeaveCriticalSection(&cs);
		//print the elements of array
		std::cout << std::endl << "Elements of array:" << std::endl;
		for (int i = 0; i < arrSize; i++)
			std::cout << arr[i] << " ";
		std::cout << std::endl;
		//print still available markers
		printf("Available markers:");
		for (int i = 1; i <= numOfMarkerThreads+1; i++)
			for (int j = 0; j < numOfMarkerThreads; j++)
				if (numbers[j] == i)
					std::cout << i << " ";
		std::cout << std::endl;
		std::cout << "Enter the number of the marker thread, that will be given the signal to finish its work ";
		std::cout << std::endl;
		std::cout << "If there are no available threads input any number ";
		int k;
		std::cin >> k;		
		int j = 0;
		//find the thread by its number
		while ((numbers[j] != k) && (j < numOfMarkerThreads))
		{
			j++;
		}
		//wrong number of thread
		if (j == numOfMarkerThreads) 
			break;
		//signal for j thread to stop working
		SetEvent(hStopEvent[j]);
		WaitForSingleObject(hMarker[j], INFINITE);
		//delete this thread
		//replace j thread and numOfMarkerThreads thread
		CloseHandle(hMarker[j]);
		hMarker[j] = hMarker[numOfMarkerThreads - 1];
		hStopEvent[j] = hStopEvent[numOfMarkerThreads - 1];
		hWorkEvent[j] = hWorkEvent[numOfMarkerThreads - 1];
		hFailEvent[j] = hFailEvent[numOfMarkerThreads - 1];
		numbers[j] = numbers[numOfMarkerThreads - 1];
		numOfMarkerThreads--;
		//print array
		std::cout << "Array: ";
		for (int i = 0; i < arrSize; i++)
			std::cout << arr[i] << " ";
		std::cout << std::endl;
		if (numOfMarkerThreads == 0)
		{
			check = false;
		}
	}

	//close critical section
	DeleteCriticalSection(&cs);
	delete[]arr;
	system("pause");
	return 0;
}