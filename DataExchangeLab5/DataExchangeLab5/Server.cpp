#include <Windows.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <vector>
struct employee
{
    int num; // id
	std::string name; // name
    double hours; // working hours
};
void getEmployees(std::vector<employee>& employees) {
    int num;
	std::string name;
    double hours;
    do {
		std::cout << "Input employee's name, input 0 to exit:\n";
        std::cin >> name;
        if (name.compare("0") == 0)
            break;
		std::cout << "Input employee's id:\n";
        std::cin >> num;
		std::cout << "Input employee's working number:\n";
        std::cin >> hours;
		bool b =true;
		for(unsigned int i=0; i<employees.size(); i++)
		{
			if(employees[i].num==num)
				b= false;
		}
			if (b) {
				employee temp;
				temp.name = name;
				temp.num = num;
				temp.hours = hours;
				employees.push_back(temp);
			}
		
    } while (true);
    return;
}
int main()
{
	//creation of binary file
	std::cout << "Input bin file name:\n";
    char binFileName[50];
    std::cin >> binFileName;
    std::fstream file(binFileName,std::ios::binary|std::ios::out);
	if (!file.is_open()) {
		std::cout << "Error while creating file!" << std::endl;
		return 0;
	}
	file.close();
	//input employees
    std::vector<employee> employees;
    getEmployees(employees);
	std::fstream fin;
	fin.open(binFileName, std::ios::binary | std::ios::in);
	std::cout << "Binary File:" << std::endl;
	//write employees to bin file and output to the screen
	for(unsigned int i=0; i<employees.size(); i++)
	{
		fin.write((char*)&employees[i], sizeof(employee));
		std::cout << employees[i].name << std::endl;
		std::cout << employees[i].num << std::endl;
		std::cout << employees[i].hours << std::endl;
	}
	fin.close();
	system("pause");
    return 0;
}