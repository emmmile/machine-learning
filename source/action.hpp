#ifndef ACTION_HPP
#define ACTION_HPP

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "random.hpp"
using namespace std;

typedef unsigned int uint;
typedef unsigned char uchar;

template<uint NSymbols, uint NStates, class TSymbol = uchar, class TState = uchar>
class action {
	bool direction;		// assume false == LEFT, true == RIGHT
	TSymbol symbol;		// symbol to write on the tape
	TState state;		// next state
public:
	action ( ) {
		direction = false;
		symbol = 0;
		state = 0;
	}

	action ( Random& gen ) {
		this->random_shuffle( gen );
	}

	action& random_shuffle ( Random& gen ) {
		direction = gen.integer() % 2;
		symbol = gen.integer() % NSymbols;
		state = gen.integer() % NStates;

		return *this;
	}

	friend ostream& operator<< ( ostream& os, const action& a ) {
		return os << (uint) a.symbol
			  << (a.direction ? "R" : "L")
			  << (uint) a.state;
	}
};


#endif // ACTION_HPP
