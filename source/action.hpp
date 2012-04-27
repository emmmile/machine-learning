#ifndef ACTION_HPP
#define ACTION_HPP

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "random.hpp"
#include "utils.hpp"
using namespace std;



template<class TState, class TSymbol>
class action {
	bool dir;		// assume false == LEFT, true == RIGHT
	TSymbol symbol;		// symbol to write on the tape
	TState state;		// next state

	// choose uniformly one of the other possibilities
	inline TSymbol change_symbol ( uint NSymbols, Random& gen ) {
		return ( symbol + 1 + ( gen.integer() % ( NSymbols - 1 ) ) ) % NSymbols;
	}

	inline TState change_state ( uint NStates, Random& gen ) {
		return ( state + 1 + ( gen.integer() % NStates ) ) % (NStates + 1);
	}

public:
	action ( ) {
		dir = false;
		symbol = 0;
		state = 0;
	}

	action (uint NStates, uint NSymbols, Random& gen ) {
		this->random_shuffle( NStates, NSymbols, gen );
	}

	action ( const action& a ) {
		dir = a.dir;
		symbol = a.symbol;
		state = a.state;
	}

	action& random_shuffle ( uint NStates, uint NSymbols, Random& gen ) {
		dir = gen.integer() % 2;
		symbol = gen.integer() % NSymbols;
		state = gen.integer() % ( NStates + 1 );

		return *this;
	}

	//mutation affects only one of the variables (XXX or all the variables??)
	//mutation can also leave the action unhaltered, is ok??
	action& mutate ( uint NStates, uint NSymbols, Random& gen ) {
		uint dice = gen.integer() % 3;
		// in this way we modify something for sure
		if ( dice == 0 ) dir = !dir;
		if ( dice == 1 ) symbol = change_symbol( NSymbols, gen );
		if ( dice == 2 ) state = change_state( NStates, gen );

		/* this can leave the action unmodified because actually can
		// choose also the current value
		if ( dice == 0 ) dir = gen.integer() % 2;
		if ( dice == 1 ) symbol = gen.integer() % NSymbols;
		if ( dice == 2 ) state = gen.integer() % NStates;*/

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
};


#endif // ACTION_HPP
