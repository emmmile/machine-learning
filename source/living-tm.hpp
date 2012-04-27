#ifndef LIVINGTM_HPP
#define LIVINGTM_HPP
/* Definition of the class living_tm about living machines, ie the ones
 * we will run (not only the transition table but also the tape, the
 * head position, etc.)
 */
#include <deque>
#include "turing-machine.hpp"

#define INIT_TAPE_SIZE 101
#define TAPE_RESIZE_STEP 50
#define EXPECTED_S 6 // expected max number of steps when known

typedef unsigned int _int; // maybe we will need more than this (e.g. unsigned long long int)

template<class TState = uchar, class TSymbol = uchar>
class living_tm {
  turing_machine<TState, TSymbol> machine;
  TState current_state;
  _int age; // age of the machine (number of evolution steps since creation)
  deque<TSymbol> tape; // tape of the machine
  _int hp; // head pointer (index in tape)
  _int nb_shifts; // number of shifts (or steps) done without halting so far
  double fitness; // last computed value of the fitness function

public:
  typedef action<TState, TSymbol> action_type;
  typedef turing_machine<uint, uint> tm_type;

  living_tm(TState NSt, TSymbol NSym) : machine( NSt, NSym ) {
    age = 0;
    current_state = 0;
    tape.resize(INIT_TAPE_SIZE, 0);
    hp = INIT_TAPE_SIZE / 2;
    nb_shifts = 0;
    fitness = update_fit();
  }

  living_tm(tm_type tm) : machine( tm ) {
    age = 0;
    current_state = 0;
    tape.resize(INIT_TAPE_SIZE, 0);
    hp = INIT_TAPE_SIZE / 2;
    nb_shifts = 0;
    fitness = update_fit();
  }
  
  living_tm(TState NSt, TSymbol NSym, Random& gen) : machine( NSt, NSym, gen ) {
    age = 0;
    current_state = 0;
    tape.resize(INIT_TAPE_SIZE, 0);
    hp = INIT_TAPE_SIZE / 2;
    nb_shifts = 0;
    fitness = update_fit();
  }

  bool do_nsteps(_int nsteps) {
    /* execute nsteps steps, return true is the machine is still
     * running, false else
     */

    action_type a;
    bool running = (current_state < machine.states());
    // ^- if the machine is running or not

    for (_int i = 0 ; i < nsteps && running ; ++i) {

      // get the next action to do :
      a = machine.step(current_state, tape[hp]);
      
      tape[hp] = a.next_symbol();
      current_state = a.next_state();
      
      if (a.direction()) {
	// go right
	if (hp == tape.size() - 1) // if we reach the end of the tape ...
	  tape.resize(tape.size() + TAPE_RESIZE_STEP, 0); // ... we expand it;
	++hp;
      } else if (!hp) // if we reach the beginning of the tape ...
	tape.push_front(0); // ... we add one cell at the beginning
      else --hp; // go left
      
      ++nb_shifts;
      
      running = current_state < machine.states();
    }
    return running;
  }

  double update_fit() {
    // update the value of fitness and return it
    if (nb_shifts < EXPECTED_S)
      fitness = 1.0 / (EXPECTED_S - nb_shifts);
    // ^- may be changed for a better one
    else if (nb_shifts == EXPECTED_S && current_state == machine.states())
      // we found it !
      clog << "=== FOUND MACHINE ===\n"
	   << machine.states() << " states, " << machine.symbols() << "symbols\n"
	   << "halts after " << nb_shifts << endl
	   << machine;
    else
      fitness = 0.0;

    return fitness;
  }

  // these 5 methods returns the private member variables,
  TState get_state() {
    return current_state;
  }

  _int get_age() {
    return age;
  }

  _int get_nb_shifts() {
    return nb_shifts;
  }

  double get_fitness() {
    return fitness;
  }

  friend ostream& operator<< ( ostream& os, const living_tm& ltm ) {

  os << "=========================\n"
     << "Number of symbols:\t" <<  ltm.machine.symbols() << endl
     << "Number of states:\t" <<  ltm.machine.states() << endl
     << "Age:\t\t\t" << ltm.age << endl
     << "Current state:\t\t" << (int) ltm.current_state << endl
     << "Current symbol:\t\t" << (int) ltm.tape[ltm.hp] << endl
     << "Head position:\t\t" << ltm.hp << endl
     << "Tape size:\t\t" << ltm.tape.size() << endl
     << "Computed steps so far:\t" << ltm.nb_shifts << endl
     << "Last computed fitness:\t" << ltm.fitness << endl
     << "Transition table:\n"
     << ltm.machine
     << "=========================\n";

    return os;
  }

};

#endif // LIVINGTM_HPP
