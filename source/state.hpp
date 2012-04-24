#ifndef STATE_HPP
#define STATE_HPP


#include "action.hpp"


template<uint NSymbols, uint NStates, class TSymbol = uchar, class TState = uchar>
class state {
	action<NSymbols, NStates, TSymbol, TState> actions [NSymbols];
public:
	state ( ) {
	}

	state& random_shuffle ( Random& gen ) {
		for ( TSymbol i = 0; i < NSymbols; ++i )
			actions[i].random_shuffle( gen );
		return *this;
	}

	friend ostream& operator<< ( ostream& os, const state& s ) {
		for ( uint i = 0; i < NSymbols; ++i )
			os << s.actions[i] << " ";
		return os;
	}
};



#endif // STATE_HPP
