/* Name : population.hpp
 * Description : Evolution simulator for Turing machines
 */
#ifndef POPULATION_HPP
#define POPULATION_HPP

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
  Random gen;

public:
  typedef living_tm<NStates, NSymbols> ltm_type;

  population() {
    machines.resize(INIT_POPULATION_SIZE);
    for (uint i = 0; i < machines.size(); ++i)
      machines[i] = new living_tm<NStates, NSymbols> (gen);
  }

  population(uint pop_size) {
    machines.resize(pop_size);
    for (uint i = 0; i < machines.size(); ++i)
      machines[i] = new living_tm<NStates, NSymbols> (gen);
  }

  inline ltm_type& operator[](const uint i) {
    // return the i-th machine in the population
    return *machines[i];
  }

  inline size_t size() const {
    return machines.size();
  }

  void push_back(const ltm_type& ltm) {
    // add the living_tm ltm in the population (at the end of the array)
    ltm_type new_machine = new ltm_type (ltm);
    machines.push_back(new_machine);
  }

  void erase(const uint i) {
    // remove the i-th machine from the population
    assert(i < size());
    delete machines[i];
    // here is better to first move the empty slot at the end
    swap( machines[i], machines.back() );
    // and then remove from the end, since we have O(1) there (and no constraints on the order)
    machines.pop_back();
  }

  void nsteps_for_all(_int n) {
    // do n step on all machines of the population
    for(int i = 0; i < size() ; ++i)
      //machines[i]->do_nsteps(n);
      this[i].do_nsteps(n);
  }

  void erase_halt() {
    // remove from the population all already halted machines
    for(int i = 0; i < size() ; ++i)
      if (this[i].ishalt())
	erase(i);
  }

  void mutate(uint i) {
    this[i].mutate(gen);
  }

  void mutate() {
    this[gen.integer() % size()].mutate(gen);
    // ^- I hope that the max of gen.integer() is greater than size()...
    // not equiprobabilistic since size does not always divide
    // the max of gen.integer()
  }

  inline void crossover(uint i_ltm1, uint i_ltm2, crossover_type type = TWO_POINT) {
        this[i_ltm1].crossover(this[i_ltm2], gen, type);
  }

  void crossover(crossover_type type = TWO_POINT) {
    int i_ltm1 = gen.integer() % size(),
      i_ltm2 = gen.integer() % size();
    crossover(i_ltm1, i_ltm2, type);
  }

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & machines;
  }

};


#endif // POPULATION_HPP
