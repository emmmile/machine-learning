#include <iostream>
#include <fstream>
#include "living-tm.hpp"
using namespace std;


// Random is class that I normally use when I need random numbers.
// it uses boost random generators that, they say, are a lot better on
// high dimensions problems (and actually are also very very fast and
// also thread safe in this way :P)

int main() {

  /*
   * Test of serialization
   */

	// usage is quite simple
	Random gen;
	living_tm<3,5> t (gen);

	// create and open a character archive for output
	std::ofstream ofs("backup_file");

	cout << t;
	
	t.do_nsteps(50);

	{
	  boost::archive::text_oarchive oa(ofs);
	  // write class instance to archive
	  oa << t;
	}

	living_tm<3,5> t2;

	{
	  // create and open an archive for input
	  std::ifstream ifs("backup_file");
	  boost::archive::text_iarchive ia(ifs);
	  // read class state from archive
	  ia >> t2;
    }
	cout << t2;

	return 0;
}
