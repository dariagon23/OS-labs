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
	std::fstream finn;
	finn.open(binFileName, std::ios::binary | std::ios::out);
	std::cout << "Binary File:" << std::endl;
	//write employees to bin file
	for(unsigned int i=0; i<employees.size(); i++)
	{
		finn.write((char*)&employees[i], sizeof(employees[i]));
	}
	finn.close();
	std::fstream in(binFileName, std::ios::in);
	struct employee emp;
	//reading from binary file
	for (unsigned int i=0; i<employees.size(); i++)
	{
		
		in.read((char*)&emp, sizeof(employee));
		std::cout <<"Id: "<< emp.num << std::endl;
		std::cout <<"Name: "<< emp.name << std::endl;
		std::cout << "Hours: " << emp.hours << std::endl;
		
	}
	in.close();
	system("pause");
    return 0;
}