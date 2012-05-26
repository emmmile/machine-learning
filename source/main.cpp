#include <iostream>
#include "living-tm.hpp"
#include "population.hpp"
#include "limits.hpp"
#include "vect.hpp"
using namespace std;


#define DIMS		100
#define GENERATIONS	3000

// test with the ackley function
typedef vect<double,DIMS> testvect;

double ackley ( const testvect& v ) {
	double a = 20;
	double b = 0.2;
	double c = 2.0 * M_PI;

	double x = 0.0;
	double y = 0.0;

	for ( uint i = 0; i < v.dims; ++i ) {
		x += v[i] * v[i];
		y += cos( v[i] * c );
	}

	return -(-a * exp( -b * sqrt( 1.0 / v.dims * x ) ) - exp( 1.0 / v.dims * y ) + a + M_E);
}

void mutate ( testvect& v, Random& gen ) {
  double step = 1.0;
  v[gen.integer() % v.dims] += step * gen.realnegative();
}

void crossover ( testvect& a, testvect& b, Random& gen ) {
  for ( uint i = gen.integer() % a.dims; i < a.dims; ++i )
    swap( a[i], b[i] );
}

void populationTest ( ) {
 
}


int main() {
  population<testvect> test;
  
  cout << "Testing the ackley problem in " << DIMS << " dimensions. Running " << GENERATIONS << " generations.\n"; 
  test.run( GENERATIONS, ackley, mutate, crossover );
  cout << "Best fitness (-value of ackley function): " << test.get_best_fitness() << endl;

  return 0;
}
