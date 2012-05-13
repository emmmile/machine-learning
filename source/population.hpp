/* Name : population.hpp
 * Description : Evolution simulator for Turing machines
 */
#ifndef POPULATION_HPP
#define POPULATION_HPP
#define SIGMOID_LAMBDA 5

#include <type_traits>
#include "random.hpp"


template<class I>
struct Fitness {
  inline double operator() ( I& i ) {
    return i.fitness();
  }
};

template<class I>
struct Mutate {
  inline void operator() ( I& i, Random& gen ) {
    i.mutate( gen );
  }
};

template<class I>
struct Crossover {
  inline void operator() ( I& a, I& b, Random& gen, crossover_type type = TWO_POINT ) {
    a.crossover( b, gen, type );
  }
};

template<typename T>
struct has_generation_fun {
  // crazy stuff.. But it works, and actually is also very efficient (compile time)!
  // http://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error
  // http://stackoverflow.com/questions/87372/is-there-a-technique-in-c-to-know-if-a-class-has-a-member-function-of-a-given

  // ::value is set to true only if it's defined a function "void T::generation (uint)"
  template<typename U, void (U::*)(uint)> struct SFINAE {};
  template<typename U> static char Test(SFINAE<U, &U::generation>*);
  template<typename U> static int Test(...);
  static const bool value = sizeof(Test<T>(0)) == sizeof(char);
};

// default Generation function caller, it does nothing!
template<class I, class Enable = void>
struct Generation {
  inline void operator() (I&, uint ) {
  }
};

// You cannot have a function like:
//   if ( has_generation_fun<I>::value ) { call to generation } else { do nothing }
// because the if is evaluated at runtime, that is: the compiler try anyway to compile
// what follows the if, both branches, that can result in a call to the non-defined function generation().
// That's why we need a specializatin of the above, that is enabled if and only if the class I has
// a generation function with the signature we want. The signature is defined in the
// struct has_generation_fun, written above.
// http://www.boost.org/doc/libs/1_49_0/libs/utility/enable_if.html
template<class I>
struct Generation<I, typename enable_if<has_generation_fun<I>::value>::type > {
  inline void operator() (I& i, uint gen ) {
    i.generation( gen );
  }
};


template<class I>
class population {
  class triple {
  public:
    I* individual;
    bool changed;
    double fitness;

    triple( ) : changed( true ), fitness( -100 ) { }
    triple( I* i, bool b, double d = -100.0 ) : individual(i), changed(b), fitness(d) { }
    inline bool operator< (const triple& another ) const {
      return fitness > another.fitness;
    }
  };

  vector<triple> individuals;
  Random gen;
  double pmutation;
  double pcrossover;

  const static uint maximumPopulation = 400;
  const static uint initialPopulation = 100;
  constexpr static double default_pcrossover = 0.9;
  constexpr static double default_pmutation = 0.1;



  void erase(const uint i) {
    // remove the i-th machine from the population
    assert(i < individuals.size());
    delete individuals[i].individual;
    // here is better to first move the empty slot at the end
    swap( individuals[i], individuals.back() );
    // and then remove from the end, since we have O(1) there (and no constraints on the order)
    individuals.pop_back();
  }

public:
  population(uint pop_size = initialPopulation, double pc = default_pcrossover, double pm = default_pmutation) {
    individuals.resize(pop_size);
    for (uint i = 0; i < individuals.size(); ++i)
      individuals[i].individual = new I (gen); // a random initializer must exist in ::I

    pmutation = pm;
    pcrossover = pc;
    gen.seed( (uint) this );
  }

  ~population( ) {
    for (uint i = 0; i < individuals.size(); ++i)
      delete individuals[i].individual;
  }

  I& get_best ( ) const {
    return *individuals[0].individual;
  }

  double get_best_fitness ( ) const {
    return individuals[0].fitness;
  }

  // The function takes as parameters the 4 functions/functors described above. For more details:
  // http://stackoverflow.com/questions/1174169/function-passed-as-template-argument
  // Called without arguments it will call the default functions inside ::I, otherwise the function/functor
  // passed as argument.
  template<typename F = Fitness<I>, typename M = Mutate<I>, typename C = Crossover<I>, typename G = Generation<I> >
  void run ( uint generations, F fitness = F(), M mutate = M(), C crossover = C(), G generation = G() ) {

    for ( uint i = 0; i < generations; ++i ) {
      // this executes the genetic operators
      genetic_operators( mutate, crossover );

      // this execute the (probabilistic) selection step
      selection( fitness, generation, i );
    }
  }

  template<typename M, typename C>
  void genetic_operators( M mutate, C crossover ) {
    uint lastSize = individuals.size();

    /* if an individual have to crossover (because of a random choice), we
     * store a pointer (partner) to it to be able to find it when an other
     * one will want to crossover.
     * When this pointer is NULL, it means that nobody wants to crossover at
     * this time.
     */
    static I* partner = NULL;

    // important: the size increases during execution, so we have to stop
    // when we finish the OLD (current) population
    for ( uint j = 0; j < lastSize; ++j ) {
      if ( gen.real() < pmutation ) {
        // creates a copy (in our case with a new, empty, tape)
        I* newone = new I( *individuals[j].individual );
        // executes mutation on the NEW copy and push it to the end
        mutate( *newone, gen );
        individuals.push_back( triple( newone, true ) );
      }

      if ( gen.real() < pcrossover ) {
        // executes crossover
	if ( !partner )
	  // no one wants to crossover at this time so we remember that this guy wants to crossover
	  partner = individuals[j].individual;
	else {
	  // someone wants to crossover
	  I* newone = new I( *individuals[j].individual );
	  I* newtwo = new I( *partner );
	  crossover( *newone, *newtwo, gen );
	  individuals.push_back( triple( newone, true ) );
	  individuals.push_back( triple( newtwo, true ) );
	  partner = NULL; // now, nobody wants to crossover
	}
      }
    }
  }

  template<typename F, typename G>
  void selection ( F fitness, G generation, uint generationNumber ) {

    for (uint j = 0; j < individuals.size(); ++j) {
      generation( *individuals[j].individual, generationNumber );

      // if needed, call the fitness and store the result
      if ( individuals[j].changed ) {
        individuals[j].fitness = fitness( *individuals[j].individual );
        individuals[j].changed = false;
      }
    }

    sort( individuals.begin(), individuals.end() );

    // some individuals die in an accident :
    for (uint i = 0; i < individuals.size(); ++i) {
      if (early_death(i)) erase(i);
    }

  }

  bool early_death(uint indiv_rank) {
    /* decides if a given individual will die or not at this generation
     * using a sigmoid function that gives to the worst individual
     * (w.r.t. its fitness) a bigger probability to die.
     * Define SIGMOID_LAMDA decides the shape of the sigmoid function
     * /!\ assumption is made that individuals are sorted
     */
    
    return (gen.real() < 1 / (1 + exp(SIGMOID_LAMBDA * ((individuals.size() - 1) / 2 - indiv_rank))));
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
