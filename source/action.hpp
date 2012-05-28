#ifndef ACTION_HPP
#define ACTION_HPP

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <assert.h>
#include "random.hpp"
#include "utils.hpp"
using namespace std;

template<uint NStates,
	 uint NSymbols,
	 class TState = state<uchar,NStates>,
	 class TSymbol = symbol<uchar,NSymbols>,
	 class TDirection = direction>
class action {
  TDirection dir;		// assume false == LEFT, true == RIGHT
  TSymbol symbol;		// symbol to write on the tape
  TState state;		// next state

  // choose uniformly one of the other possibilities
  inline TSymbol change_symbol ( Random& gen ) {
    return ( symbol + 1 + ( gen.integer() % ( NSymbols - 1 ) ) ) % NSymbols;
  }

  inline TState change_state ( Random& gen ) {
    return ( state + 1 + ( gen.integer() % NStates ) ) % (NStates + 1);
  }

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & dir;
    ar & symbol;
    ar & state;
  }

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

  static uint offset ( uint index ) {
    switch( index ) {
      case 0:
        return offsetof( action, dir );
      case 1:
        return offsetof( action, symbol );
      case 2:
      default:
        return offsetof( action, state );
    }
  }

  static uint sizeOf ( uint index ) {
    switch( index ) {
      case 0:
        return sizeof( dir );
      case 1:
        return sizeof( symbol );
      case 2:
      default:
        return sizeof( state );
    }
  }

  action (TState st, TSymbol sy, TDirection d) {
    //  assert(sy < NSymbols && st <= NStates);

    if (!(sy < NSymbols)) {
      	cout << "/!\\ "<< sy <<" >= " << NSymbols;
    exit(-1);
  } else if (!(st <= NStates)) {
	cout << "/!\\ "<< st <<" > " << NStates;
	exit(-1);
      }
    dir = d;
    symbol = sy;
    state = st;
  }

  action& random_shuffle ( Random& gen ) {
    dir = gen.integer() % 2;
    symbol = gen.integer() % NSymbols;
    state = gen.integer() % ( NStates + 1 );

    return *this;
  }

  //mutation affects only one of the variables (XXX or all the variables??)
  //mutation can also leave the action unhaltered, is ok??
  action& mutate ( Random& gen ) {
    uint dice = gen.integer() % 3;
    // in this way we modify something for sure
    if ( dice == 0 ) dir = !dir;
    if ( dice == 1 ) symbol = change_symbol( gen );
    if ( dice == 2 ) state = change_state( gen );

    /* this can leave the action unmodified because actually can
    // choose also the current value
    if ( dice == 0 ) dir = gen.integer() % 2;
    if ( dice == 1 ) symbol = gen.integer() % NSymbols;
    if ( dice == 2 ) state = gen.integer() % NStates;*/

    return *this;
  }

  // these methods returns the private member variables,
  // if you want to avoid them, simply make the 3 variables public
  TDirection direction( ) const {
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
