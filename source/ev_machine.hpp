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
};


#endif // EVMACHINE_HPP
