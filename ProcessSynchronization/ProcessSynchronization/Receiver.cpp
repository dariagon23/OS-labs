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
	return 0;
}