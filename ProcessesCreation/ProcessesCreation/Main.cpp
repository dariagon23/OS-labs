#include <windows.h>
#include <conio.h>
#include<iostream>
int main()
{
	char binFileName[20];
	std::cin >> binFileName;
	int count;
	std::cin >> count;

	char lpszCommandLine[40] = "Creator.exe ";
	strcat(lpszCommandLine, binFileName);
	char buff[10];
	strcat(lpszCommandLine, " ");
	strcat(lpszCommandLine ,itoa(count, buff, 10));
	STARTUPINFO si;
	PROCESS_INFORMATION piCom;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	// ������� ����� ���������� �������
	CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE,
	CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCom);
	//�������� ��������� ��������
	WaitForSingleObject(piCom.hProcess, INFINITE);
	// ��������� ����������� ����� ��������
	CloseHandle(piCom.hThread);
	CloseHandle(piCom.hProcess);
	_cputs("The new process is created.\n");
	_cputs("Press any key to finish.\n");
	_getch(); 
	return 0;
}
