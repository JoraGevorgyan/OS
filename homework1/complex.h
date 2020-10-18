#pragma once  // if this file is already included in the project,
              // this will stop doing it again

struct complex {  // there's struct, because its object by default are public
        complex(double a = 0, double b = 0) // objects parameters
                                            // by default 0
                : a(a)
                 ,b(b)
        {
        }
        complex operator + (complex);
        complex operator - (complex);
        complex operator * (complex);
        void print();// printing an objec like "a +bi"
        complex mulByConst(double); // multiply the complex objec by real number
        double abs();  // counting the absalutely value of an object
        double a;
        double b;
};

namespace comp {  // this namespace will help me, to know,
                  // that, these f-s are for complex objects (arrays)
	complex* mkArray(int); // this will make an array of complex,
                           // on the given length
	void print(complex*, int);
	void sort(complex*, int); // workes with QuickSort algorithm
}

