#ifndef TURINGMACHINE_HPP
#define TURINGMACHINE_HPP

#include "action.hpp"
#include "state.hpp"


template<uint NSymbols, uint NStates, class TSymbol = uchar, class TState = uchar>
class turing_machine {
	state<NSymbols, NStates, TSymbol, TState> states [NStates];

public:
	turing_machine( ) {
	}

	turing_machine( Random& gen ) {
		this->random_shuffle( gen );
	}

	turing_machine& random_shuffle ( Random& gen ) {
		for ( TState i = 0; i < NStates; ++i )
			states[i].random_shuffle( gen );
		return *this;
	}

	friend ostream& operator<< ( ostream& os, const turing_machine& tm ) {
		os << "   ";
		// only works for small numbers!
		for ( TSymbol i = 0; i < NSymbols; ++i )
			os << (uint) i << ":  ";
			os << endl;

		for ( TState i = 0; i < NStates; ++i )
			os << (uint) i << ": " << tm.states[i] << endl;

		return os;
	}
};

#endif // TURINGMACHINE_HPP
