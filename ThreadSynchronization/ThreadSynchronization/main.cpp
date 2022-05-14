#include<iostream>
#include<vector>
int arrSize;
int*arr;
HANDLE *hWorkEvent, *hStopEvent, *hFailEvent;
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

	//close critical section
	DeleteCriticalSection(&cs);
	delete[]arr;
	system("pause");
	return 0;
}