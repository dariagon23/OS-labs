#include <iostream>
#include <Employee.h>
#include <Windows.h>
#pragma once
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
#define _CRT_SECURE_NO_WARNINGS

std::wstring convertToWideString(const std::string& str) {
	const char* c_str = str.c_str();
	wchar_t* wstr = new wchar_t[str.length() + 1];
	mbstowcs(wstr, c_str, str.size() + 1);
	std::wstring wide(wstr);
	delete[] wstr;
	return wide;
}
struct ThreadArgs {
	size_t number_of_clients;
	std::string filename;

	const std::vector<HANDLE>& read_events;
	const std::vector<HANDLE>& modify_events;

	std::vector<size_t> read_section_counter;

	const std::vector<HANDLE>& mutexes;

	ThreadArgs(size_t num_of_clients, std::string file, const std::vector<HANDLE>& r_events, const std::vector<HANDLE>& m_events, const std::vector<HANDLE>& muts) :
	number_of_clients(num_of_clients), filename( file ), read_events( r_events ), modify_events( m_events ), read_section_counter(r_events.size(), 0), mutexes( muts ) {}
};

int main() {
	HANDLE hPipe;

	while (true) {
		hPipe = CreateFile(L"\\\\.\\pipe\\my_pipe", GENERIC_ALL, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hPipe != INVALID_HANDLE_VALUE) {
			break;
		}
		if (GetLastError() != ERROR_PIPE_BUSY) {
			std::cerr << "Could not open pipe: " << GetLastError();
			system("pause");
			return -1;
		}
		if (!WaitNamedPipe(L"\\\\.\\pipe\\my_pipe", 20000)) {
			std::cerr << "could not open pipe: 20 second wait timeout\n";
			system("pause");
			return -1;
		}
	}
	char input;
	DWORD bytes_written, bytes_read;
	employee empl;
	int id;

	std::cout << "1) m - modification\n2) r - read file\n3) q - exit\n";
	std::cout << "Enter: ";
	while (std::cin >> input) {
		if (!WriteFile(hPipe, &input, 1, &bytes_written, NULL)) {
			std::cerr << "Error writing to pipe: " << GetLastError();
			return -1;
		}
		if (input != 'm' && input != 'r') {
			break;
		}
		std::cout << "Enter id: ";
		std::cin >> id;
		if (!WriteFile(hPipe, &id, sizeof(int), &bytes_written, NULL) || bytes_written != sizeof(int)) {
			std::cerr << "Error writing to pipe: " << GetLastError();
			return -1;
		}
		if (!ReadFile(hPipe, &empl, sizeof(employee), &bytes_read, NULL) || bytes_read != sizeof(employee)) {
			std::cerr << "Error reading from pipe: " << GetLastError();
			return -1;
		}
		std::cout << empl << '\n';
		if (input == 'm') {
			std::cout << "--------------------------------------------------\n";
			std::cout << "Enter name: ";
			std::cin >> empl.name;
			std::cout << "Enter hours: ";
			std::cin >> empl.hours;
			if (!WriteFile(hPipe, &empl, sizeof(employee), &bytes_written, NULL)) {
				std::cerr << "Error writing to pipe: " << GetLastError();
				return -1;
			}
		}

		std::cout << "Press m for modification, r for read, q to exit process: ";
	}
	CloseHandle(hPipe);
}