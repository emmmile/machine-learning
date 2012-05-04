#ifndef TURINGMACHINE_HPP
#define TURINGMACHINE_HPP

#include <assert.h>
#include <boost/archive/text_oarchive.hpp> // for serialization
#include <boost/archive/text_iarchive.hpp> // idem
#include <boost/serialization/vector.hpp> // idem
#include <boost/serialization/deque.hpp> // idem


#include "action.hpp"

enum crossover_type {
  TWO_POINT,		// choose 2 actions at random and swap what is in between
  ONE_POINT,		// choose 1 action at random and swap what is at the right

  // I don't know if these two are useful, maybe before implementing them it is useful
  // to try the first two, and then, after, if we have time or we want to compare, try also these ones
  TWO_POINT_IN_ACTION,	// choose 2 actions and 2 points in every of them, then swap the middle
  ONE_POINT_IN_ACTION	// choose 1 action and 1 point inside it, then swap what is at the right
};

// TState and TSymbol are now used only for storing (inside the action class),
// for iterating over the elements I actually use uints
template<uint NStates, uint NSymbols,
   class TState = state<uchar,NStates>,
   class TSymbol = symbol<uchar,NSymbols>,
   class TDirection = direction>
class turing_machine {
  action<NStates, NSymbols, TState, TSymbol, TDirection> actions [NStates * NSymbols];

  inline uint size ( ) {
    return NStates * NSymbols;
  }

  inline uint random_pos( Random& gen ) {
    return gen.integer() % size();
  }

  inline turing_machine& swap_actions ( uint beg, uint end, turing_machine& a ) {
    if ( beg > end ) swap( beg, end );
    // last position MUST be inclusive, that is swap the range [beg,end], not [beg,end)
    swap_ranges( actions + beg, actions + end + 1, a.actions + beg );
    return *this;
  }

  // choose two cells at random
  // and swap everything that is in the middle (swap the actions)
  inline turing_machine& crossover_two_point ( turing_machine& a, Random& gen ) {
    uint beg = random_pos( gen );
    uint end = random_pos( gen );
    return swap_actions( beg, end, a );
  }

  // choose one cell at random and swap everything at the right
  inline turing_machine& crossover_one_point ( turing_machine& a, Random& gen ) {
    uint beg = random_pos( gen );
    uint end = size() - 1;
    return swap_actions( beg, end, a );
  }
  
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    // it would be good to serialize NStates and NSymbols to,
    //but it's constants so it fails
    uint states = NStates;
    uint symbols = NSymbols;
    ar & states;
    ar & symbols;
    // this is of course true when we are writing, it's important only during loading
    assert( states == NStates && symbols == NSymbols );
    ar & actions;
  }

public:
  typedef action<NStates, NSymbols, TState, TSymbol, TDirection> action_type;
  typedef TSymbol symbol_type;
  typedef TState state_type;
  typedef TDirection direction_type;

  turing_machine( ) {
  }

  turing_machine( Random& gen ) {
    this->random_shuffle( gen );
  }

  turing_machine& random_shuffle ( Random& gen ) {
    for ( uint i = 0; i < size(); ++i )
      actions[i].random_shuffle( gen );
    return *this;
  }



  // returns the action relative to the symbol sym, if we are in state s
  action_type& operator() ( const TState s, const TSymbol sym ) {
    return step( s, sym );
  }

  // the same
  action_type& step ( const TState s, const TSymbol sym ) {
    // the following assert will fail is we ask the action relative to
    // any symbol if we are in state NState (halt state) but the function
    // do_step (of living_tm) take care of this
    assert( s < NStates && sym < NSymbols );
    return actions[s * NSymbols + sym];
  }



  turing_machine& crossover ( turing_machine& a, Random& gen, crossover_type type = TWO_POINT ) {
    switch( type ) {
    case TWO_POINT:
      return crossover_two_point( a, gen );
    case TWO_POINT_IN_ACTION:
      return crossover_two_point( a, gen ); // to be implemented
    case ONE_POINT:
      return crossover_one_point( a, gen );
    case ONE_POINT_IN_ACTION:
      return crossover_one_point( a, gen ); // to be implemented
    }

    return crossover_two_point( a, gen );
  }

  // mutation: takes a single action and mutate a single variable inside it
  turing_machine& mutate ( Random& gen ) {
    actions[random_pos(gen)].mutate( gen );
    return *this;
  }



  friend ostream& operator<< ( ostream& os, const turing_machine& tm ) {
    uint symw = ndigits10( NSymbols );	// symbol max width
    uint stw = ndigits10( NStates );	// state max width
    uint aw = symw + stw + 2;		// action max width

    os << setw( aw - stw - 2 ) << " ";

    for ( TSymbol i = 0; i < NSymbols; ++i )
      os << setw( aw ) << i << ":";
      os << endl;

    for ( TState i = 0; i < NStates; ++i ) {
      os << "S" << setw( stw ) << i << ": ";
      for ( TSymbol j = 0; j < NSymbols; ++j )
        os << tm.actions[i * NSymbols + j].next_symbol()
           << tm.actions[i * NSymbols + j].direction()
           << "S" << tm.actions[i * NSymbols + j].next_state() << " ";
      os << endl;
    }

    return os;
  }
};

#endif // TURINGMACHINE_HPP
