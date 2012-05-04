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
  _int age;

public:
  typedef living_tm<NStates, NSymbols> ltm_type;

  population() {
    age = 0;
    machines.resize(INIT_POPULATION_SIZE);
    for (int i = 0; i < INIT_POPULATION_SIZE; ++i)
      machines[i] = new living_tm<NStates, NSymbols>;
  }

  population(Random& gen) {
    age = 0;
    machines.resize(INIT_POPULATION_SIZE);
    for (int i = 0; i < INIT_POPULATION_SIZE; ++i)
      machines[i] = new living_tm<NStates, NSymbols> (gen);
  }

  ltm_type operator[](const uint i) {
    // return the i-th machine in the population
    assert(i < machines.size());
    return *machines[i];
  }

  void push_back(const ltm_type& ltm) {
    // add the living_tm ltm in the population (at the end of the array)
    ltm_type new_machine = new ltm_type (ltm);
    machines = push_back(&new_machine);
  }

  void erase(const uint i) {
    // remove the i-th machine from the population
    assert(i < machines.size());
    delete *machines[i];
    machines.erase(i);
  }



};


#endif // EVMACHINE_HPP
