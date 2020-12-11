#include <iostream>
#include "complex.h" //for complex objects and f-s

int main() {
	double a, b; // for making complex object
    std::cout << "Type two complex numbers without 'i' (f.e. for a + bi type just a and b)\n";
	std::cin >> a >> b;
	complex x(a, b); // making first comlex object with, like a + bi
	std::cin >> a >> b; // for second complex number
	complex y(a, b); // making second complex number
	std::cout << "x + y == "; //printing sum of two complex numbers
	(x + y).print();
	std::cout << '\n';
	
	std::cout << "Type the length of the array\n\n";
	int length; // the length of array, that must be sorted
	std::cin >> length;
	complex* cArr = comp::mkArray(length); // calling the f-n,
                                           //that will be make an array of complex objects
	std::cout << "Array is\n\n";
	comp::print(cArr, length); // calling the f-n, that will be print an array
	std::cout << "\nThe sorted array is\n\n";
	comp::sort(cArr, length);  // calling the f-n, that wiil be sort an array
	comp::print(cArr, length); // printing array
	delete[] cArr;
}

