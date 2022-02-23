#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <conio.h>
#include<iostream>
#include<fstream>
using namespace std;
struct employee
{
int num; // идентификационный номер сотрудника
char name[10]; // имя сотрудника
double hours; // количество отработанных часов
};
int main()
{
    setlocale(LC_ALL, "Russian");
	char binFileName[20];
	std::cout << "Input bin file name >" << std::endl;
	std::cin >> binFileName;
	std::cout << "Input the count of empls >" << std::endl;
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
	// создаем новый консольный процесс
    CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE,
	CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCom);
	//ожидание окончание процесса
	WaitForSingleObject(piCom.hProcess, INFINITE);
	// закрываем дескрипторы этого процесса
	CloseHandle(piCom.hThread);
	CloseHandle(piCom.hProcess);
	//чтение из бинарного файла
	fstream in(binFileName, ios::in);
	struct employee emp;
	for (int i=0; i<count; i++)
	{
		
		in.read((char*)&emp, sizeof(employee));
		std::cout << emp.num << " ";
		std::cout << emp.name << " ";
		std::cout << emp.hours << std::endl;
		
	}
	in.close();

	std::cout << "Input the name of report file > " << std::endl;
	char reportName[20];
	std::cin >> reportName;
	std::cout << "Input the pay per hour > " << std::endl;
	int pay;
	std::cin >> pay;
	char lpszCommandLine2[40] = "Reporter.exe ";
	strcat(lpszCommandLine2, binFileName);
	strcat(lpszCommandLine2, " ");
	strcat(lpszCommandLine2, reportName);
	char buff2[10];
	strcat(lpszCommandLine2, " ");
	strcat(lpszCommandLine2 ,itoa(pay, buff2, 10));

	STARTUPINFO si2;
	PROCESS_INFORMATION piCom2;
	ZeroMemory(&si2, sizeof(STARTUPINFO));
	si2.cb = sizeof(STARTUPINFO);
	// создаем новый консольный процесс
    CreateProcess(NULL, lpszCommandLine2, NULL, NULL, FALSE,
	CREATE_NEW_CONSOLE, NULL, NULL, &si2, &piCom2);
	//ожидание окончание процесса
	WaitForSingleObject(piCom2.hProcess, INFINITE);
	// закрываем дескрипторы этого процесса
	CloseHandle(piCom2.hThread);
	CloseHandle(piCom2.hProcess);
	//
	ifstream finn(reportName);
	while (!finn.eof())
	{
		char* str = new char[100];
		finn.getline(str, 100);
		cout << str << endl;
		delete[]str;
	}
	finn.close();
	_getch(); 
	return 0;
}
