#include<iostream>
struct employee {
	int num;
	char name[20];
	double hours;

	friend std::istream& operator>>(std::istream& in, employee& emp) {
		in >> emp.num >> emp.name >> emp.hours;
		return in;
	}

	friend std::ostream& operator<<(std::ostream& out, const employee& emp) {
		out << emp.num << ' ' << emp.name << ' ' << emp.hours;
		return out;
	}
};