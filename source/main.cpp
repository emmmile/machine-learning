#include <iostream>
#include <fstream>
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

  // create and open a character archive for output
  std::ofstream ofs("backup_file");
  cout << t;

  t.do_nsteps(50);
  boost::archive::text_oarchive oa(ofs);
  // write class instance to archive
  oa << t;
  ofs.close();

  living_tm<6,2> t2;

  // create and open an archive for input
  std::ifstream ifs("backup_file");
  boost::archive::text_iarchive ia(ifs);
  // read class state from archive
  ia >> t2;
  cout << t2;

  cout << turing_machine<5,5>::spacesize() << endl;
  cout << turing_machine<3,2>::spacesize() << endl;
  cout << turing_machine<6,2>::spacesize() << endl;
  cout << turing_machine<7,9>::spacesize() << endl;

  return 0;
}
