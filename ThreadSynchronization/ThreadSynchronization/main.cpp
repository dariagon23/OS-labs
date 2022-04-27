#include<iostream>
int size;
int main()
{
	std::cin >> size;
	int*arr=new int[size];
	for(int i=0; i<size;i++)
		arr[i]=0;
	for(int i=0; i<size; i++)
		std::cout << arr[i]<<" ";
	std::cout << std::endl;
	delete[]arr;
	system("pause");
	return 0;
}