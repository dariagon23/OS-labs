#include<windows.h>
#include<iostream>
int n;
DWORD WINAPI average(LPVOID num)
{
	int avrg = 0;
	for(int i=0; i<n;  i++)
	{
		avrg+=((int*)num)[i];
		Sleep(12);
	}
	avrg/=n;
	std::cout << "Average value = " <<avrg << std::endl; 
	return 0;
} 
DWORD WINAPI min_max(LPVOID num)
{
	int ind_min = 0, ind_max = 0;
	for(int i=1; i<n;  i++)
	{
		if(((int*)num)[i]<((int*)num)[ind_min])
		{
			ind_min = i;
			Sleep(7);
		}
		else
		{
			if(((int*)num)[i]>((int*)num)[ind_max])
			{
				ind_max = i;
				Sleep(7);
			}
		}
	}
	std::cout << "Max value = " <<((int*)num)[ind_max] << std::endl;
	std::cout << "Min value = " <<((int*)num)[ind_min] << std::endl;
	return 0;
} 
int main()
{
	std::cin >> n;
	int*arr = new int[n];
	for(int i=0; i<n; i++)
	{
		std::cin >> arr[i];
	}
	HANDLE hAverage;
	DWORD IDAverage;
	// Average thread creation
	hAverage = CreateThread(NULL, 0, average, (void*)arr, 0, &IDAverage);
	if (hAverage == NULL)
		return GetLastError();
	// waiting for average 
	WaitForSingleObject(hAverage, INFINITE);
	// close average
	CloseHandle(hAverage);

	HANDLE hMinMax;
	DWORD IDMinMax;
	// MinMax thread creation
	hMinMax = CreateThread(NULL, 0, min_max, (void*)arr, 0, &IDMinMax);
	if (hMinMax == NULL)
		return GetLastError();
	// waiting for min_max 
	WaitForSingleObject(hMinMax, INFINITE);
	// close min_max
	CloseHandle(hMinMax);
	system("pause");
	return 0;
}