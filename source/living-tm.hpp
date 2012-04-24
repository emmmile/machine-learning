#ifndef LIVINGTM_HPP
#define LIVINGTM_HPP

#include "turing-machine.hpp"

#define INIT_TAPE_SIZE 100

typedef unsigned long int _int; // maybe we will need more than uint (e.g. unsigned long long int)

template<uint NStates, uint NSymbols, class TState = uchar, class TSymbol = uchar>
class living_tm {
  turing_machine<NStates, NSymbols, uint, uint> tm;
  TState current_state;
  _int age; // age of the machine (number of evolution steps since creation)
  vector<TSymbol> tape; // tape of the machine
  _int hp; // head pointer (index in tape)
  _int nb_shifts; // number of shifts (or steps) done without halting so far
  double fitness; // last computed value of the fitness function
public:
  /*
  void shift ( ) {
      turing_machine<NStates, NSymbols, uint, uint>::action_type a = tm( state, tape_symbol )
      }*/

  // these methods returns the private member variables,
  _int get_age ( ) {
    return age;
  }

  _int get_nb_shifts ( ) {
    return nb_shifts;
  }

  double get_fitness ( ) {
    return fitness;
  } 
};

#endif // LIVINGTM_HPP
