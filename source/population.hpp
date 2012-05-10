/* Name : population.hpp
 * Description : Evolution simulator for Turing machines
 */
#ifndef POPULATION_HPP
#define POPULATION_HPP

#include "living-tm.hpp"
#define INIT_POPULATION_SIZE 100
#define DEFAULT_PMUTATION 0.01
#define DEFAULT_PCROSSOVER 0.7



/*
  X Jean-Florent:
  I wanted to make this generic, so maybe we will use it also in other contexts.
  I take as template parameter the type of the idividuals (can be vectors, turing machines, whatever..).
  In our case of course I == living-tm<N,M>.

  Then for run() it I need 3 functions, by default members of I:: (see also living-tm.hpp):
   - a fitness() function, takes an individual and returns a double
   - a mutate() function, takes an individual and modifies it
   - a crossovers() function, takes two individuals and modifies them
 */
template<class I>
class population {
  vector<I*> individuals;
  Random gen;
  double pmutation;
  double pcrossover;

  struct Compare {
    inline bool operator() ( const I* a, const I* b ) {
      // the operator< on the I instances (not the pointers) must be defined
      return *a < *b;
    }
  } compareIndividuals;

public:
  population(uint pop_size = INIT_POPULATION_SIZE, double pc = DEFAULT_PCROSSOVER, double pm = DEFAULT_PMUTATION) {
    individuals.resize(pop_size);
    for (uint i = 0; i < individuals.size(); ++i)
      individuals[i] = new I (gen);

    pmutation = pm;
    pcrossover = pc;
  }

  ~population( ) {
    for (uint i = 0; i < individuals.size(); ++i)
      delete individuals[i];
  }

  inline I& operator[](const uint i) {
    // return the i-th machine in the population
    return *individuals[i];
  }

  inline size_t size() const {
    return individuals.size();
  }

  void push_back(const I& ltm) {
    // add the living_tm ltm in the population (at the end of the array)
    I* new_machine = new I (ltm);
    individuals.push_back(new_machine);
  }

  void erase(const uint i) {
    // remove the i-th machine from the population
    assert(i < size());
    delete individuals[i];
    // here is better to first move the empty slot at the end
    swap( individuals[i], individuals.back() );
    // and then remove from the end, since we have O(1) there (and no constraints on the order)
    individuals.pop_back();
  }

  // moved all executing stuff in living-tm, since they depend on that type.

  // moved all the crossover and mutate functionalities in living-tm, since dependes on
  // that type, not on the population (the aim is to have population completely general).



  // The function takes as parameters the 3 functions/functors described above. For more details:
  // http://stackoverflow.com/questions/1174169/function-passed-as-template-argument
  // Called without arguments it will call the default functions inside ::I, otherwise the function/functor
  // passed as argument.
  // This c++ mechanism is really new for me, and I think is the one used also for the std::sort()
  // or other STL algorithms (that takes user-defined functions, but they have also a default one).
  template<typename F = Fitness<I>, typename M = Mutate<I>, typename C = Crossover<I> >
  void run ( uint generations, F fitness = F(), M mutate = M(), C crossover = C() ) {

    for ( uint i = 0; i < generations; ++i ) {
      // this executes the genetic operators
      genetic_operators( mutate, crossover );

      // this should execute the (probabilistic) selection step, using the provided fitness function
      // eventually deleting some individuals from the population
      selection( fitness );
    }
  }

  template<typename M, typename C>
  void genetic_operators( M mutate, C crossover ) {
    uint lastSize = individuals.size();

    // important: the size increases during execution, so we have to stop
    // when we finish the OLD (current) population
    for ( uint j = 0; j < lastSize; ++j ) {
      if ( gen.real() < pmutation ) {
        // creates a copy (in our case with a new, empty, tape)
        I* newone = new I( *individuals[j] );
        // executes mutation on the NEW copy and push it to the end
        mutate( *newone, gen );
        individuals.push_back( newone );
      }

      if ( gen.real() < pcrossover ) {
        // executes crossover
        // TODO which other individual use for crossover?
        uint another = gen.integer() % individuals.size();

        I* newone = new I( *individuals[j] );
        I* newtwo = new I( *individuals[another] );
        crossover( *newone, *newtwo, gen, TWO_POINT );
        individuals.push_back( newone );
        individuals.push_back( newtwo );
      }
    }
  }

  // probabilistic selection, this is where we need to execute the machines and see how they
  // behave. The fitness function should cause a re-execution if the machine has changed, nothing
  // if the machine hasn't been mutated or crossover-ed, and more steps if the machine has been left
  // behind (I would leave this case at the end, because it seems to me the more complicated to implement).
  template<typename F>
  void selection ( F fitness ) {

    double best = 0.0;
    uint bestIndividual = 0;


    for (uint j = 0; j < individuals.size(); ++j) {
      // this should recompute the fitness value if necessary, executing more steps if necessary
      // Maybe passing also the generation number for implementig the k steps increment
      double f = fitness( *individuals[j] );

      if ( f > best ) {
        bestIndividual = j;
        best = f;
      }
    }

    sort( individuals.begin(), individuals.end(), compareIndividuals );

    for (uint j = 0; j < individuals.size(); ++j){
      if ( )
        erase(j);
    }

    cout << "Best fitness is " << best << ".\n";
    cout << "Best individual is " << bestIndividual << ".\n";
  }


  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & individuals;
    ar & pmutation;
    ar & pcrossover;
  }

};


#endif // POPULATION_HPP
