#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <stdint.h>
#include <math.h>

#define USE_BOOST		1

#if USE_BOOST
#include <boost/random.hpp>
#endif

// RAND_MAX on windows is 0x7FFF
#ifdef _WIN32
#undef RAND_MAX
#define RAND_MAX 0x7FFFFFFF
#endif

class Random {
private:
#if USE_BOOST
  // the best are mt19937, taus88, rand48 (in order of goodness->slowness)
  typedef boost::mt19937 random_generator;
  random_generator generator;

  inline int32_t gen ( ) {
    return generator() & RAND_MAX;
  }
#else
  uint32_t x, y, z, w;

  inline int32_t gen ( ) {
    uint32_t t;

    t = x ^ (x << 11);
    x = y; y = z; z = w;
    return ( w = w ^ (w >> 19) ^ (t ^ (t >> 8)) ) & RAND_MAX;
  }
#endif
public:
  Random ( uint32_t seed = 123456789 ) {
#if USE_BOOST
    generator.seed( seed );
#else
    x = seed;
    y = 362436069;
    z = 521288629;
    w = 88675123;
#endif
  }

  // get uniformly an integer in [0,RAND_MAX)
  int32_t integer ( ) {
    return gen();
  }

  // get uniformly a real in [0,1)
  double real ( ) {
    return gen() / (double) RAND_MAX;
  }

  // get uniformly a real in (-1,1)
  double realnegative ( ) {
    return ( gen() << 1 ) / (double) RAND_MAX;
  }

  // get uniformly a real in [0,2)
  double real2 ( ) {
    return gen() / RAND_MAX * 2.0;
  }

  // get uniformly a real in (-2,2)
  double real2negative ( ) {
    return ( gen() << 1 ) / RAND_MAX * 2.0;
  }

  int32_t operator() ( int32_t n ) {
    return gen() % n;
  }

  // change the seed
  void seed ( uint32_t seed ) {
#if USE_BOOST
    generator.seed( seed );
#else
    x = seed;
#endif
  }
};


#endif // RANDOM_H
