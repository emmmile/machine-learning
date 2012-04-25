#ifndef TURINGMACHINE_HPP
#define TURINGMACHINE_HPP

#include "action.hpp"




template<uint NStates, uint NSymbols, class TState = uchar, class TSymbol = uchar>
class turing_machine {
	//state<NStates, NSymbols, TState, TSymbol> states [NStates];
	action<NStates, NSymbols, TState, TSymbol> actions [NStates][NSymbols];
public:
	typedef action<NStates, NSymbols, TState, TSymbol> action_type;
	typedef TSymbol symbol_type;
	typedef TState state_type;

	turing_machine( ) {
	}

	turing_machine( Random& gen ) {
		this->random_shuffle( gen );
	}

	turing_machine& random_shuffle ( Random& gen ) {
		for ( TState i = 0; i < NStates; ++i )
			for ( TSymbol j = 0; j < NSymbols; ++j )
				actions[i][j].random_shuffle( gen );
		return *this;
	}

	// returns the action relative to the symbol sym, if we are in state s
	action_type& operator() ( const TState s, const TSymbol sym ) {
		return step( s, sym );
	}

	// the same
	action_type& step ( const TState s, const TSymbol sym ) {
		assert( s < NStates && sym < NSymbols );
		return actions[s][sym];
	}

	friend ostream& operator<< ( ostream& os, const turing_machine& tm ) {
		uint wst = ndigits(NStates - 1);
		uint wsy = ndigits(NSymbols - 1);

		os << string( wst + 2, ' ' );
		for ( TSymbol i = 0; i < NSymbols; ++i )
			os << setw( wsy + 1 ) << print_sym( i ) << ":" << string( wst + 1, ' ' );
			os << endl;

		for ( TState i = 0; i < NStates; ++i ) {
			os << setw( wst + 1 ) << print_state( i ) << ": ";
			for ( TSymbol j = 0; j < NSymbols; ++j )
				os << tm.actions[i][j] << " ";
			os << endl;
		}

		return os;
	}
};

#endif // TURINGMACHINE_HPP
