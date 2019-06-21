#include <iostream>
#include "Scripts/MainActivity.h"

int main(int argc, char* args[]) {
	std::cout << "Main Activity Started!" << std::endl;

	MainActivity mainActivity("Factory", 1024, 600);
	mainActivity.~MainActivity();

	/*
	std::cout << "Type 'exit' to end program" << std::endl << ">> ";
	while (true) {
		std::string a = "";
		std::cin >> a;
		if (a == "exit") break;
	}
	*/

	return EXIT_SUCCESS;
}