/*****************************************************************************
 * random_search.cpp                                                               *
 *****************************************************************************/

#include <iostream>
#include <ctime>
#include <cstring>

#include "living-tm.hpp"
#include "limits.hpp"
#include "vect.hpp"
#include "nm.hpp" // change machine size in this file (M: alphabet, N: states)

using namespace std;

int main() {

  typedef living_tm<N, M> ltm;
  ltm m;
  Random gen;
  uint explored = 0;
  cout << "Finding BB(" << M << ", "<< N << ") with random search...\n";  


  if (slimits<N, M>::has_upper)  
    do {
      m = ltm(gen);
      ++explored;
      //      if (explored % 1000 == 0)
      //	cout << "\rExplored more than " << explored << " machines..."; 
      
      m.do_nsteps(slimits<N, M>::upper);

    } while (!m.get_state().ishalt() || m.get_nb_shifts() != slimits<N, M>::upper);
    else {
      cout << "Only implemented for machines where upper bound is known.\n";
      return EXIT_FAILURE;
    }

  cout << "\nFound a machine after " << explored << " tries:\n" << m; 

  return EXIT_SUCCESS;
}

