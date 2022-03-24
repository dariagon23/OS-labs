#include<windows.h>
#include<iostream>
volatile int avrg,n;
DWORD WINAPI average(LPVOID num)
{
	avrg =0;
	for(int i=0; i<n;  i++)
	{
		avrg+=((int*)num)[i];
	}
	avrg/=n;
	return 0;
} 
int main()
{
	int temp;
	std::cin >> temp;
	n = temp;
	int*arr = new int[n];
	for(int i=0; i<n; i++)
	{
		std::cin >> arr[i];
	}
	HANDLE hMain;
	DWORD IDMain;
	// Main thread creation
	hMain = CreateThread(NULL, 0, average, (void*)arr, 0, &IDMain);
	if (hMain == NULL)
		return GetLastError();
	// waiting for average 
	WaitForSingleObject(hMain, INFINITE);
	// close average
	CloseHandle(hMain);
	std::cout << "Average value = " <<avrg << std::endl; 
	return 0;
}