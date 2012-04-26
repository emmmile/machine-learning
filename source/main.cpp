#include <iostream>
#include "living-tm.hpp"
using namespace std;


// Random is class that I normally use when I need random numbers.
// it uses boost random generators that, they say, are a lot better on
// high dimensions problems (and actually are also very very fast and
// also thread safe in this way :P)

int main() {
  //	typedef turing_machine<9, 8> tm;

	// usage is quite simple
	Random gen;

	living_tm<3,5> t (gen);

	cout << t;
	
	int s = 1;
	bool running = true;
	while (s && running) {
	  cout << "How many steps (0 for stop) ? ";
	  cin >> s;
	  running = t.do_nsteps(s);
	  cout << t;
	}

	/*
	// random initialization
	tm t ( gen );
	tm s ( gen );
	cout << s << endl;
	cout << t << endl;
	t.crossover( s, gen );
	cout << t << endl;


	tm::state_type state = 1;
	tm::symbol_type tape_symbol = 4;
	// get an action
	tm::action_type a = t( state, tape_symbol ); // same as t.step( state, tape_symbol );

	cout << "From state " << print_state( state )
	     << ", having " << print_sym( tape_symbol ) << " on the tape, "
	     << "we go in state " << print_state( a.next_state() ) << ", writing "
	     << print_sym( a.next_symbol() ) << " and moving "
	     << print_dir( a.direction() ) << endl;

	// this should cause an error
	//t( 8, 2 );
	*/

	return 0;
}
