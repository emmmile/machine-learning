#include <iostream>
#include "living-tm.hpp"
#include "population.hpp"
using namespace std;


int main() {
  Random gen;

  // serialization test
  living_tm<6,2> t (gen);

  t.do_nsteps(50);
  cout << t;
  t.save( "backup_file" );

  living_tm<6,2> t2( "backup_file" );
  cout << t2;

  // space size test
  cout << turing_machine<6,2>::spacesize() << endl;


  // population test
  population<6,2> p;

  //p.run();

  return 0;
}
