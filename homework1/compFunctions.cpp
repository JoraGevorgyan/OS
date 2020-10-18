#include "complex.h" // for complex object and f-s
#include <iostream>  // for stdin, stdout
#include <cmath>    // for sqrt in comp::abs()

complex complex::operator + (complex tmp) {
        complex res;
        res.a = a + tmp.a;
        res.b = b + tmp.b;
        return res;
}

complex complex::operator - (complex tmp) {
        complex res;
        res.a = a - tmp.a;
        res.b = b - tmp.b;
        return res;
}

complex complex::operator * (complex tmp) {
        complex res;
        res.a = a * tmp.a - b * tmp.b;
        res.b = a * tmp.b + tmp.a * b;
        return res;
}

void complex::print() {
    if(a) { // if a is 0, it becomes useless to print it
        printf("%.2lf ", a);
    }
	if(b) {   // if b is 0, it becomes useless to print it
		if(b > 0) { // if b is positive, I need to print '+' symbol
			std::cout << "+";
		}
		printf("%.2lfi", b);
	}
}

complex complex::mulByConst(double value) {
	complex res;
	res.a = a * value;
	res.b = b * value;
	return res;
}

double complex::abs() {
	return sqrt(a*a + b*b);// the absolutlye value of complex numbers is always a real number
}

int partSort(complex*, int, int); // this must be called from "sort" f-n
void quickSort(complex*, int, int); // must be called from "sort" f-n

complex* comp::mkArray(int l) {// making an array of complex objects
    //  srand(time(NULL));
        complex* arr = new complex[l];
        for(int i = 0; i < l; ++i) {
                int x = rand() % 100 + 1 - rand() % 100; // for the beauty I taked integer numbers
                arr[i].a = x / 4.0; // in this way they becomes to real numbers
                x = rand() % 100 + 1 - rand() % 100;
                arr[i].b = x / 4.0;
        }
	return arr;
}

void comp::print(complex* arr, int l) {
        for(int i = 0; i < l; ++i) {
		arr[i].print(); // calling print f-n for complex object
		std::cout << "  ";
	}
	std::cout << '\n';
}

void comp::sort(complex* arr, int l) {
	for(int i = 0; i < l; ++i) { // this loop will make all members of array in absolutely value
		arr[i].a = arr[i].abs();
		arr[i].b = 0;
	}
	quickSort(arr, -1, l - 1); // calling quickSort for the array
}

int partSort(complex* arr, int left, int right) {
        complex tmp(arr[right].a, 0); // pivot for sorting
        int index = left - 1; // current index for start
        for(int i = left; i < right; ++i) {
                if(arr[i].a < tmp.a) {/// if the pivot is greater than current element, swap these 
                        ++index;
                        std::swap(arr[index].a, arr[i].a);
                }
        }
        std::swap(arr[index + 1].a, arr[right].a); //all elements in the left of current are less,
                                                   // than pivot, and others are greater than pivot
                                                   // so, I need swap these elements
        return index + 1;
}

void quickSort(complex* arr, int left, int right) {
        if(left < right) { // if left >= right it meens, that, all array is sorted
                int index = partSort(arr, left, right); // partsort will return the index of pivot
                                                        // with that index i will multiply array by
                                                        // two parts, and sort these apart
                quickSort(arr, left, index - 1);
                quickSort(arr, index + 1, right);
        }
}
