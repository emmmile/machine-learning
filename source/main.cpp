
#include <iostream>
#include "turing-machine.hpp"
using namespace std;


// Random is class that I normally use when I need random numbers.
// it uses boost random generators that, they say, are a lot better on
// high dimensions problems (and actually are also very very fast and
// also thread safe in this way :P)

int main() {
	typedef turing_machine<8, 9, uint, uint> tm;

	// usage is quite simple
	Random gen;

	// random initialization
	tm t ( gen );
	cout << t << endl;

	// get an action
	uint state = 5;
	uint tape_symbol = 3;
	tm::action_type a = t( state, tape_symbol ); // same as t.step( state, tape_symbol );

	cout << "From state " << state << ", having " << tape_symbol << " on the tape, "
	     << "we go in state " << a.next_state() << ", writing "
	     << a.next_symbol() << " and moving "
	     << getdir( a.direction() ) << endl;

	// this should cause an error
	//t( 8, 2 );


	return 0;
}
