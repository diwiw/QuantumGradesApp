#include "Grades.h"
#include "version.h"
#include <iostream>

int main() {
	std::cout << "===================================\n";
	std::cout << " QuantumGradesApp\n";
	std::cout << " Version: " << APP_VERSION << "\n";
	std::cout << " Build date: " << APP_BUILD_DATE << "\n";
	std::cout << "===================================\n\n";
	Grades g;
	g.add(5);
	g.add(4);
	g.add(3);
	g.print();
	return 0;
}
