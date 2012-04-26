#include <iostream>
#include "living-tm.hpp"
using namespace std;


int main() {

	/* test the new data structures
	int array[10];fill( array, array + 10, 9999 );
	typeholder<int> a( 3.2 );
	a = 2;
	state<uchar> b = 7;
	cout << a << " " << array[b] << endl;

	for ( state<uchar,7> i = 0; i < 10; ++i ) {
		cout << i << endl;
	}*/

	Random gen;
	living_tm<3,8> t (gen);
	t.do_nsteps( 100000000);


	return 0;
}
