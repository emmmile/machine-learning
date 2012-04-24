#ifndef TURINGMACHINE_HPP
#define TURINGMACHINE_HPP

#include "action.hpp"
#include "state.hpp"


template<uint NStates, uint NSymbols, class TState = uchar, class TSymbol = uchar>
class turing_machine {
	state<NStates, NSymbols, TState, TSymbol> states [NStates];
public:
	typedef action<NStates, NSymbols, TState, TSymbol> action_type;


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

	// returns the action relative to the symbol sym, if we are in state s
	action_type& operator() ( const TState s, const TSymbol sym ) {
		return step( s, sym );
	}

	// the same
	action_type& step ( const TState s, const TSymbol sym ) {
		assert( s < NStates && sym < NSymbols );
		return states[s][sym];
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
