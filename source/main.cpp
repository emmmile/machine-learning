
#include <iostream>
#include "turing-machine.hpp"
using namespace std;


// Random is class that I normally use when I need random numbers.
// it uses boost random generators that, they say, are a lot better on
// high dimensions problems (and actually are also very very fast and
// also thread safe in this way :P)

int main() {
	typedef turing_machine<6, 2> tm;

	// usage is quite simple
	Random gen;

	// random initialization
	tm t ( gen );
	cout << t << endl;

	tm::state_type state = 5;
	tm::symbol_type tape_symbol = 1;
	// get an action
	tm::action_type a = t( state, tape_symbol ); // same as t.step( state, tape_symbol );

	cout << "From state " << print_state( state )
	     << ", having " << print_sym( tape_symbol ) << " on the tape, "
	     << "we go in state " << print_state( a.next_state() ) << ", writing "
	     << print_sym( a.next_symbol() ) << " and moving "
	     << print_dir( a.direction() ) << endl;

	// this should cause an error
	//t( 8, 2 );


	return 0;
}
