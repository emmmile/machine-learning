#include <iostream>
#include "living-tm.hpp"
#include "population.hpp"
using namespace std;

typedef living_tm<6,2> ltm;


double externalFitnessFunction( ltm& a ) {
  return 1.0;
}

int main() {
  // space size test
  cout << turing_machine<6,2>::spacesize() << endl;


  // population test to show that you can easily pass external fitness functions
  population<ltm> p;
  p.run( 1, externalFitnessFunction );
  p.run( 1 );

  //population<int> another;

  return 0;
}
