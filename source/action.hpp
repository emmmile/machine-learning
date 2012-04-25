#ifndef ACTION_HPP
#define ACTION_HPP

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "random.hpp"
#include "utils.hpp"
using namespace std;



template<uint NStates, uint NSymbols, class TState, class TSymbol>
class action {
	bool dir;		// assume false == LEFT, true == RIGHT
	TSymbol symbol;		// symbol to write on the tape
	TState state;		// next state
public:
	action ( ) {
		dir = false;
		symbol = 0;
		state = 0;
	}

	action ( Random& gen ) {
		this->random_shuffle( gen );
	}

	action ( const action& a ) {
		dir = a.dir;
		symbol = a.symbol;
		state = a.state;
	}

	action& random_shuffle ( Random& gen ) {
		dir = gen.integer() % 2;
		symbol = gen.integer() % NSymbols;
		state = gen.integer() % NStates;

		return *this;
	}

	// these methods returns the private member variables,
	// if you want to avoid them, simply make the 3 variables public
	bool direction( ) const {
		return dir;
	}

	TSymbol next_symbol( ) const {
		return symbol;
	}

	TState next_state( ) const {
		return state;
	}

	friend ostream& operator<< ( ostream& os, const action& a ) {
		return os << setw( ndigits( NSymbols - 1 ) ) << print_sym( a.symbol )
			  << print_dir( a.dir )
			  << setw( ndigits( NStates - 1 ) + 1 ) << print_state( a.state );
	}
};


#endif // ACTION_HPP
