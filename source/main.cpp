#include <iostream>
#include "living-tm.hpp"
using namespace std;


int main() {
	Random gen;
	living_tm<3, 8> t (gen);

	cout << t;
	t.do_nsteps( 200000000 );

	return 0;
}
