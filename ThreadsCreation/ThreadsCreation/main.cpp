#include<windows.h>
#include<iostream>
DWORD WINAPI Note(LPVOID num)
{
	std::cout << "Thread is created" << std::endl;
	//(int)num++;
	std::cout << (int)num << std::endl;
	return 0;
} 
int main()
{
	int n = 100;
	HANDLE hMain;
	DWORD IDMain;
	// Main thread creation
	 hMain = CreateThread(NULL, 0, Note, (void*)n, 0, &IDMain);
	if (hMain == NULL)
		return GetLastError();
	// waiting for Note 
	WaitForSingleObject(hMain, INFINITE);
	// close Note
	CloseHandle(hMain);
	std::cout << n << std::endl;
	std::cout << "The End!"<< std::endl; 
	return 0;
}