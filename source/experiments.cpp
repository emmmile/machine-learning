/*****************************************************************************
 * experiments.cpp                                                           *
 * principal program we use to launch experiments on BB and EC               *
 * main.cpp is used for tests and bug solving                                *
 *****************************************************************************/

#include <iostream>
#include <ctime>
#include <cstring>

#include "living-tm.hpp"
#include "population.hpp"
#include "limits.hpp"
#include "vect.hpp"
#include "nm.hpp" // change machine size in this file (M: alphabet, N: states)

#define PATH_SIZE 255 // /!\ overflow if it is not big enough
#define SAVE_PATH "../results/" //place where to save data
// ^--- /!\ don't omit final slash

using namespace std;


int main() {

  typedef living_tm<N, M> ltm;

  population<ltm> p; // our population of TM
  int generations = 0;

  time_t rawtime; // used to get time to choose file names
  struct tm * timeinfo; // idem
  char file_path[PATH_SIZE];
  char time_str[PATH_SIZE];

  cout << "*** TUREV EXPERIMENTS ***\n\n"
       << "Size of the alphabet:\t" << M
       << "\nNumber of operational states:\t" << N
       << "\nSearch space size:\t" << ltm::spacesize() << "\n\n";

  while (1) {
    cout << "How many generations (-1 for stop) ? ";
    cin >> generations;
    if (generations < 0) break;
    
    p.run(generations);
    cout << "Best machine:\n" << p.get_best() << endl;
  }

  cout << "You stopped evolution. You may want to save data.\n"
       << "\t[1] Population ;\n"
       << "\t[2] Best machine ;\n"
       << "\t[3] Both ;\n"
       << "\t[Other number] Nothing, thanks.\n"
       << "Your choice :";
  cin >> generations;

  // get time to build file names
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  sprintf(time_str, "%s%d-%d-%d_%d:%d",
	  SAVE_PATH,
	  timeinfo->tm_year,
	  timeinfo->tm_mon,
	  timeinfo->tm_mday,
	  timeinfo->tm_hour,
	  timeinfo->tm_min
	  );

  if (generations == 1 || generations == 3) {
    // build path
    strcpy(file_path, time_str);
    strcat(file_path, ".pop");

    // create and open a character archive for output
    std::ofstream ofs(file_path);
    boost::archive::text_oarchive oa(ofs);
    oa << p; // serialize
    ofs.close();
  }

  if (generations == 2 || generations == 3) {
    // build path
    strcpy(file_path, time_str);
    strcat(file_path, ".tm");

    // create and open a character archive for output
    std::ofstream ofs(file_path);
    boost::archive::text_oarchive oa(ofs);
    oa << p.get_best(); // serialize
    ofs.close();
  }

  return EXIT_SUCCESS;
}

