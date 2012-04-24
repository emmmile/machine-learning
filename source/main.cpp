
#include <iostream>
#include "turing-machine.hpp"
using namespace std;


// Random is class that I normally use when I need random numbers.
// it uses boost random generators that, they say, are a lot better on
// high dimensions problems (and actually are also very very fast and
// also thread safe in this way :P)

int main() {
	// usage is quite simple
	Random gen;
	turing_machine<8, 9> a ( gen );
	turing_machine<2, 2> b;

	cout << a << endl;
	cout << b << endl;

	return 0;
}
