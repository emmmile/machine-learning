#include <iostream>
using namespace std;
typedef unsigned char uchar;



template<uint NSymbols, uint NStates, class TSymbol = uchar, class TState = uchar>
class action {
	bool direction;
	TSymbol symbol;
	TState nextstate;

	void mutate ( ) {
		symbol = random() % NSymbols;
		state = random() % NStates;
	}
};




template<uint NSymbols, uint NStates, class TSymbol = uchar, class TState = uchar>
class state {
	action<NSymbols, NStates, TSymbol, TState> actions [NSymbols];
};




template<uint NSymbols, uint NStates, class TSymbol = uchar, class TState = uchar>
class turing_machine {
	state<NSymbols, NStates, TSymbol, TState> states [NStates];

	//circular array, std::vector or std::deque for the tape?
};




int main() {
	turing_machine<2, 6> a;
	turing_machine<22, 65> b;
	turing_machine<2, 2, bool, bool> c;
	turing_machine<2, 2> d;

	cout << sizeof( a ) << endl << sizeof( b ) << endl << sizeof( c ) << endl << sizeof( d ) << endl;



	return 0;
}

