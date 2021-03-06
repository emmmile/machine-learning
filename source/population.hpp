/* Name : population.hpp
 * Description : Evolution simulator for Turing machines
 */
#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <type_traits>
#include <boost/archive/text_oarchive.hpp> // for serialization
#include <boost/archive/text_iarchive.hpp> // idem
#include <boost/serialization/vector.hpp> // idem
#include <boost/serialization/deque.hpp> // idem
#include "random.hpp"
using namespace boost;
using namespace std;


enum crossover_type {
  TWO_POINT,		// choose 2 actions at random and swap what is in between
  ONE_POINT,		// choose 1 action at random and swap what is at the right
  UNIFORM
};


enum early_death_type {
  // type of function used to know death rate un function of rank
  // See function early_dead below.
  SIGMOID, // sigmoid function
  AFFINE,  // affine function
  STEP,    // step function (Heaviside)
  FLAT  // same rate for everyone
};


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
struct Generation<I, typename std::enable_if<has_generation_fun<I>::value>::type > {
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

    triple( ) : changed( true ), fitness( 0.0 ) { }
    triple( I* i, bool b, double d = 0.0 ) : individual(i), changed(b), fitness(d) { }
    inline bool operator< (const triple& another ) const {
      return fitness > another.fitness;
    }

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & *individual;
      ar & changed;
      ar & fitness;
  }

  };

  vector<triple> individuals;
  Random gen;
  double pmutation;
  double pcrossover;
  bool stationary;
  unsigned long int age; // /!\ this type may be too small
  unsigned long int explored; // /!\ this type may be too small

  const static uint maximumPopulation = 400;
  const static uint initialPopulation = 100;
  constexpr static double default_pcrossover = 0.9;
  constexpr static double default_pmutation = 0.8;



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
  population(uint seed = clock() + time(NULL), uint pop_size = initialPopulation,
	     bool st = true,
             double pc = default_pcrossover,
	     double pm = default_pmutation) {

    individuals.resize(pop_size);
    for (uint i = 0; i < individuals.size(); ++i)
      individuals[i].individual = new I (gen); // a random initializer must exist in ::I
    
    age = 0;
    explored = pop_size;
    pmutation = pm;
    pcrossover = pc;
    stationary = st;
    gen.seed( seed ); // clock()
  }

  ~population( ) {
    for (uint i = 0; i < individuals.size(); ++i)
      delete individuals[i].individual;
  }

  uint size() {
    return individuals.size();
  }

  I& get_best ( ) const {
    return *individuals[0].individual;
  }

  unsigned long int get_age() {
    return age;
  }

  unsigned long int get_explored() {
    return explored;
  }

  void get_stats(uint& n_halt, uint& best_nbshifts, uint& best_halted) {

    n_halt = 0;
    best_nbshifts = 0;
    best_halted = 0;
    for (unsigned i = 0; i < individuals.size(); ++i) {
      if (individuals[i].individual->get_state().ishalt()) {
	++n_halt;
	if (individuals[i].individual->get_nb_shifts() > best_halted)
	  best_halted = individuals[i].individual->get_nb_shifts();
      }
      best_nbshifts = max(best_nbshifts, individuals[i].individual->get_nb_shifts()); 
    }
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
      selection( fitness, generation, age);
      ++age;
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

    /* Previous crossover
    static I* partner;
    partner = NULL; // explicitly set to NULL at every generation, not only the first time
    */

    // important: the size increases during execution, so we have to stop
    // when we finish the OLD (current) population
    for ( uint j = 0; j < lastSize; ++j ) {
      if ( gen.real() < pmutation ) {
        // creates a copy (in our case with a new, empty, tape)
        I* newone = new I( *individuals[j].individual );
        // executes mutation on the NEW copy and push it to the end
        mutate( *newone, gen );
        individuals.push_back( triple( newone, true ) );
        explored++;
      }

      if ( gen.real() < pcrossover ) {
        // executes crossover

	uint partner_id;
	do {
	partner_id = gen.integer() % size();
	}while (partner_id == j);

	I* newone = new I( *individuals[j].individual );
	I* newtwo = new I( *individuals[partner_id].individual );
	crossover( *newone, *newtwo, gen, TWO_POINT );
	individuals.push_back( triple( newone, true ) );
	individuals.push_back( triple( newtwo, true ) );

	explored +=2;

	/* Previous crossover
	if ( !partner )
	  // no one wants to crossover at this time so we remember that this guy wants to crossover
	  partner = individuals[j].individual;
	else {
	  // someone wants to crossover
	  I* newone = new I( *individuals[j].individual );
	  I* newtwo = new I( *partner );
	  crossover( *newone, *newtwo, gen, TWO_POINT );
	  individuals.push_back( triple( newone, true ) );
	  individuals.push_back( triple( newtwo, true ) );
	  partner = NULL; // now, nobody wants to crossover
	  explored += 2;
	}
	*/
      }
    }
  }
  
  template<typename F, typename G>
  void selection ( F fitness, G generation, uint generationNumber ) {

    // run Turing machines if needed
    for (uint j = 0; j < individuals.size(); ++j) {
      generation( *individuals[j].individual, generationNumber );

      // if needed, call the fitness and store the result
      if ( individuals[j].changed || !stationary ) {
        individuals[j].fitness = fitness( *individuals[j].individual );
        individuals[j].changed = false;
      }
    }

    sort( individuals.begin(), individuals.end() );

    // Here we have to be careful.. There was some very subdle errors.
    // The idea is: iterate over every individual and if it has to die, delete it.
    // There are two problems:
    // 1) if one individual is deleted we can't increase the index i, because another individual
    // has been moved there (from erase). So we have to call early_death also for that individual.
    // 2) also with this correction we are actually wrong for two reasons (depending if we deleted
    // or not the previous individual):
    //    a) the individual now at position i can now be the last individual we had in the ranking!!
    //    b) otherwise we are using a bigger value for the sigmoid function, because it's like if we
    //       moved the whole population back of (number-of-erased-individuals) positions,
    //       while we still get the value of sigmoid(i). I verified this printing the population
    //       size, that was bigger than expected.
    //
    //for (uint i = 0; i < individuals.size(); ) {
    //  if (early_death(i)) erase(i);
    //  else  ++i;
    //}
    //
    // I think that the quicker way to implement this is to simply begin the scan from the back.
    // In this case the order of the individuals that still has to be decided, is preserved.

    // some individuals die in an accident :

    if ( individuals.size() < maximumPopulation ) return;

    int safe = 100;
    int nonSafe = individuals.size() - safe;
    double threshold = double( individuals.size() - maximumPopulation ) / nonSafe;

    for (int i = individuals.size()-1; i >= safe; --i) {
      if ( gen.real() < threshold ) erase(i);
    }

  }

  bool early_death(uint indiv_rank, early_death_type type = SIGMOID) {
    /* decides if a given individual will die or not at this generation
     * /!\ assumption is made that individuals are sorted
     *
     * we usually give the worst individual
     * (w.r.t. its fitness) a bigger probability to die.
     *
     * Four functions are possible to give a dying probability. All of
     * then are centered on the half of the population.
     *
     * Then functions can be scaled and translated on the y axis.
     * x0 = maximumPopulation - 1
     * n = individuals.size()
     *
     * Sigmoid function (type == SIGMOID) :
     *
     *     ^
     *   1 |            _____
     *     |          .-
     *     |         /
     * 0.5 |        .  parameter : lambda
     *     |       /
     *     |_____.-
     *   0 |-------------------------->
     *     0        x0     n-1
     *
     * Affine function (type == AFFINE) :
     * 
     *     ^
     *   1 |           ------
     *     |          /
     *     |         /
     * 0.5 |        /  slope coefficient : affine_coef
     *     |       /
     *     |      /
     *   0 |-----/-------------------->
     *     0        x0     n-1
     *
     * Step function (type == STEP) :
     *
     *     ^
     *   1 |
     *   h |         _______
     *     |        |
     * 0.5 |        | h : step_high
     * 1-h |________|
     *     |
     *   0 |-------------------------->
     *     0        x0     n-1
     *
     * Constant function (type == UNIFORM) :
     *
     *     ^
     *   1 |
     *     |
     *   c |----------------- 
     * 0.5 |
     *     |  c : uniform_ceil
     *     |
     *   0 |-------------------------->
     *     0        x0     n-1
     *
     *
     * lambda : parameter for the sigmoid function. bigger -> sharper slope
     * uniform_ceil : value returned for any individual
     * step_high : value of the higher step
     * affine_coef : coefficient of the slope
     *
     * vert_shift : value added to the result
     * scale : value multiplicated to the result (before adding vert_shift)
     *
     * /!\ be sure that the value is in the right range
     * when changing coefficients
     */
    const static double
      lambda = 0.01,
      uniform_ceil = 0.5,
      step_high = 0.8,
      affine_coef = 0.0005;

    const static int popCut = int (maximumPopulation - 1);


    static bool sigmoidTableReady = false;
    const static uint sigmoidTableSize = 4 * maximumPopulation;
    static double sigmoid [sigmoidTableSize]; // this should be a quite "safe" upper bound

    if ( !sigmoidTableReady ) {
      for ( uint i = 0; i < sigmoidTableSize; ++i )
	sigmoid[i] = 1 / ( 1 + exp( lambda * (popCut - int(i) )));
      sigmoidTableReady = true;
    }

      
    double res_value = 0.0; // ceil to know if an individual will die of not
    // if we draw a real in [0,1] inferior to res_value, return true,
    // else false
    
      switch (type) {
      case SIGMOID:
	res_value = sigmoid[indiv_rank];
	break;
      case AFFINE:
        res_value = affine_coef * (int(indiv_rank) - popCut) + 0.5;
	break;
      case STEP:
        if (int(indiv_rank) < popCut) res_value = step_high;
	else res_value = 1 - step_high;
	break;
      case FLAT:
        res_value = uniform_ceil;
      }

      return gen.real() < res_value;
  }

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & individuals;
    ar & pmutation;
    ar & pcrossover;
    ar & age;
  }

};

#endif // POPULATION_HPP
