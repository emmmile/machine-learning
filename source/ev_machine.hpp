/* Name : ev_machine.hpp
 * Description : Evolution simulator for Turing machines
 */
#ifndef EVMACHINE_HPP
#define EVMACHINE_HPP

#include "living-tm.hpp"
#define INIT_POPULATION_SIZE 100

template<uint NStates,
	 uint NSymbols,
	 class TState = state<uchar,NStates>,
	 class TSymbol = symbol<uchar,NSymbols>,
	 class TDirection = direction
	 >
class population {
  vector<living_tm<NStates, NSymbols>*> machines;

public:
  typedef living_tm<NStates, NSymbols> ltm_type;

  population() {
    machines.resize(INIT_POPULATION_SIZE);
    for (int i = 0; i < INIT_POPULATION_SIZE; ++i)
      machines[i] = new living_tm<NStates, NSymbols>;
  }

  population(Random& gen) {
    machines.resize(INIT_POPULATION_SIZE);
    for (int i = 0; i < INIT_POPULATION_SIZE; ++i)
      machines[i] = new living_tm<NStates, NSymbols> (gen);
  }

  ltm_type operator[](const uint i) {
    // return the i-th machine in the population
    assert(i < machines.size());
    return *machines[i];
  }

  inline ltm_type at(const uint i) {
    return *machines[i]
  }

  inline size_type size() {
    return machines.size();
  }

  void push_back(const ltm_type& ltm) {
    // add the living_tm ltm in the population (at the end of the array)
    ltm_type new_machine = new ltm_type (ltm);
    machines = push_back(&new_machine);
  }

  void erase(const uint i) {
    // remove the i-th machine from the population
    assert(i < size());
    delete at(i);
    machines.erase(i);
  }

  void nsteps_for_all(_int n) {
    // do n step on all machines of the population
    for(int i = 0; i < size() ; ++i)
      at(i).do_nsteps(n);
  }

  void erase_halt() {
    // remove from the population all already halted machines
    for(int i = 0; i < size() ; ++i)
      if (at(i).get_state() == NStates)
	erase(i);
  }

  void mutate(Random& gen) {
    at(gen.integer() % size()).mutate(gen);
    // ^- I hope that the max of gen.integer() is greater than size()...
    // not equiprobabilistic since size does not always divide
    // the max of gen.integer()
  }

  void crossover ( turing_machine& a, Random& gen, crossover_type type = TWO_POINT ) {
    int i_ltm1 = gen.integer() % size(),
      i_ltm2 = gen.integer() % size();
    at(i_ltm1).crossover(at(i_ltm2),gen,type);

  }

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & machines;
  }

};


#endif // EVMACHINE_HPP
