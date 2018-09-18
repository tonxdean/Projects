
#include <fstream>
#include <iostream>
#include <string>

int main ()
{

	std::ifstream inf("prog1.txt",std::ifstream::in);
	std::string temp;
	bool start = true;
	if (!inf.good())
	{
		std::cout << "file not good" << std::endl;
		std::cin >> temp;
		return 1;
	}


	while (inf >> temp)
	{
		std::cout << temp;
		if (temp == ";")
		{
			std::cout << std::endl;
		}
	}
	
	std::cin >> temp;
	return 0;
}