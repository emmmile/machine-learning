#ifndef STATE_HPP
#define STATE_HPP


#include "action.hpp"


template<uint NStates, uint NSymbols, class TState = uchar, class TSymbol = uchar>
class state {
	action<NStates, NSymbols, TState, TSymbol> actions [NSymbols];
public:
	state ( ) {
	}

	state& random_shuffle ( Random& gen ) {
		for ( TSymbol i = 0; i < NSymbols; ++i )
			actions[i].random_shuffle( gen );
		return *this;
	}

	action<NStates, NSymbols, TState, TSymbol>&
	operator() ( const TSymbol sym ) {
		return (*this)[sym];
	}

	action<NStates, NSymbols, TState, TSymbol>&
	operator[] ( const TSymbol sym ) {
		assert( sym < NSymbols );
		return actions[sym];
	}

	friend ostream& operator<< ( ostream& os, const state& s ) {
		for ( uint i = 0; i < NSymbols; ++i )
			os << s.actions[i] << " ";
		return os;
	}
};



#endif // STATE_HPP
