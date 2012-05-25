#ifndef LIVINGTM_HPP
#define LIVINGTM_HPP
/* Definition of the class living_tm about living machines, ie the ones
 * we will run (not only the transition table but also the tape, the
 * head position, etc.)
 */

#include <fstream>
#include <deque>
#include "turing-machine.hpp"
#include "limits.hpp"

#define INIT_TAPE_SIZE 101
//#define TAPE_RESIZE_STEP 50
//#define EXPECTED_S 6 // expected max number of steps when known

typedef unsigned int _int;
// ^- maybe we will need more than this (e.g. unsigned long long int)





template<uint NStates, uint NSymbols>
class living_tm {
  typedef state<uchar,NStates> TState;
  typedef symbol<uchar,NSymbols> TSymbol;
  typedef direction TDirection;

  turing_machine<NStates, NSymbols, TState, TSymbol, TDirection> machine;
  TState current_state;
  _int age; // age of the machine (number of evolution steps since creation)
  deque<TSymbol> tape; // tape of the machine
  _int hp; // head pointer (index in tape)
  _int nb_shifts; // number of shifts (or steps) done without halting so far
  //double lastFitness; // last computed value of the fitness function

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & machine;
    ar & current_state;
    ar & age;
    ar & tape;
    ar & hp;
    ar & nb_shifts;
    //ar & lastFitness;
  }

public:
  typedef turing_machine<NStates, NSymbols, TState, TSymbol, TDirection> tm_type;
  typedef typename tm_type::action_type action_type;
  typedef living_tm<NStates, NSymbols> ltm_type;

  living_tm() {
    age = 0;
    current_state = 0;
    tape.resize(INIT_TAPE_SIZE, 0);
    hp = INIT_TAPE_SIZE / 2;
    nb_shifts = 0;
    //lastFitness = fitness();
  }

  living_tm(tm_type tm) {
    machine = tm;
    age = 0;
    current_state = 0;
    tape.resize(INIT_TAPE_SIZE, 0);
    hp = INIT_TAPE_SIZE / 2;
    nb_shifts = 0;
    //lastFitness = fitness();
  }

  living_tm(const ltm_type& ltm)
    : living_tm( ltm.machine ) {
  }

  living_tm(const char* filename) {
    load( filename );
  }
  
  living_tm(Random& gen) {
    machine.random_shuffle(gen);
    age = 0;
    current_state = 0;
    tape.resize(INIT_TAPE_SIZE, 0);
    hp = INIT_TAPE_SIZE / 2;
    nb_shifts = 0;
    //lastFitness = fitness();
  }

  bool do_nsteps(_int nsteps) {
    /* execute nsteps steps, return true is the machine is still
     * running, false else
     */

    action_type a;
    bool running = current_state.isrunning();
    // ^- if the machine is running or not

    for (_int i = 0 ; i < nsteps && running ; ++i) {

      // get the next action to do :
      a = machine.step(current_state, tape[hp]);
      
      tape[hp] = a.next_symbol();
      current_state = a.next_state();
      
      if (a.direction().isright()) {
        // go right
        if (hp == tape.size() - 1) // if we reach the end of the tape ...
          //tape.resize(tape.size() + TAPE_RESIZE_STEP, 0); // ... we expand it;
          tape.push_back(0);
        ++hp;
      } else if (!hp) // if we reach the beginning of the tape ...
        tape.push_front(0); // ... we add one cell at the beginning
      else --hp; // go left
      
      ++nb_shifts;
      
      running = current_state.isrunning();
    }

    return running;
  }

  double fitness() {
    // if I have no upper bound I don't know what to do :P
    if ( current_state.ishalt() )
      return nb_shifts;
    else
      return 0.0;
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

  inline static double spacesize ( ) {
    return turing_machine<NStates, NSymbols>::spacesize();
  }

  // wrappers for the functions in turing-machine.hpp
  void crossover ( living_tm& a, Random& gen, crossover_type type = TWO_POINT ) {
    this->machine.crossover( a.machine, gen, type );
  }

  void mutate ( Random& gen ) {
    machine.mutate( gen );
  }

  void generation ( uint generation ) {
    if ( current_state.ishalt() ) return; // nothing to do

    if ( !slimits<NStates,NSymbols>::has_upper ) {
      do_nsteps( 10 * generation - nb_shifts ); // performs 10 steps at each generation
      return;
    }

    // try to make the number of steps performed by the S-busy beaver
    if ( slimits<NStates,NSymbols>::upper > nb_shifts )
      do_nsteps( slimits<NStates,NSymbols>::upper - nb_shifts );
  }

  friend ostream& operator<< ( ostream& os, const living_tm& ltm ) {

  os << "=========================\n"
     << "Number of symbols:\t" << NSymbols << endl
     << "Number of states:\t" << NStates << endl
     << "Age:\t\t\t" << ltm.age << endl
     << "Current state:\t\t" << ltm.current_state << endl
     << "Current symbol:\t\t" << ltm.tape[ltm.hp] << endl
     << "Head position:\t\t" << ltm.hp << endl
     << "Tape size:\t\t" << ltm.tape.size() << endl
     << "Computed steps so far:\t" << ltm.nb_shifts << endl
     //<< "Last computed fitness:\t" << ltm.lastFitness << endl
     << "Transition table:\n"
     << ltm.machine 
     << "=========================\n";
    
    return os;
  }

  void save ( const char* filename ) const {
    ofstream ofs( filename );
    boost::archive::text_oarchive oa( ofs );
    // write class instance to archive
    oa << *this;
    ofs.close();
  }

  void load ( const char* filename ) {
    ifstream ifs( filename );
    boost::archive::text_iarchive ia( ifs );
    // read class state from archive
    ia >> *this;
  }

  void set_action(const TState st, const TSymbol sy, const action_type& a) {
    // modify the transition table
    machine.set_action(st, sy, a);
  }

//  friend bool operator<(const living_tm& ltm_left, const living_tm& ltm_right) {
//    // update fitnesses and compare machines relatively to their fitness
//    // this cannot be done if they are const arguments, and I think is better to assume
//    // that when comparing the fitness is already updated
//    //ltm_right.update_fit();
//    //ltm_left.update_fit();
//    return ltm_left.get_fitness() < ltm_right.get_fitness();
//  }

//  friend bool operator>(const living_tm& ltm_left, const living_tm& ltm_right) {
//    return ltm_right < ltm_left;
//  }

//  friend bool operator<=(const living_tm& ltm_left, const living_tm& ltm_right) {
//    return ltm_left < ltm_right || ltm_left.get_fitness() == ltm_right.get_fitness();
//    // ^- evaluation of the first part of the clause will update fitness and make
//    // the second part of the clause relevant
//  }

//  friend bool operator>=(const living_tm& ltm_left, const living_tm& ltm_right) {
//    return ltm_left > ltm_right || ltm_left.get_fitness() == ltm_right.get_fitness();
//    // ^- evaluation of the first part of the clause will update fitness and make
//    // the second part of the clause relevant
//  }

};

#endif // LIVINGTM_HPP
