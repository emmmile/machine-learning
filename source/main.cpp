#include <iostream>
#include "living-tm.hpp"
#include "ev_machine.hpp"
using namespace std;


int main() {
  /*
  * Test of serialization
  */

  // usage is quite simple
  Random gen;
  living_tm<6,2> t (gen);

  t.do_nsteps(50);
  cout << t;
  t.save( "backup_file" );

  living_tm<6,2> t2( "backup_file" );
  cout << t2;

  cout << turing_machine<5,5>::spacesize() << endl;
  cout << turing_machine<3,2>::spacesize() << endl;
  cout << turing_machine<6,2>::spacesize() << endl;
  cout << turing_machine<7,9>::spacesize() << endl;

  return 0;
}
