#include <iostream>
#include "limits.hpp"
#include <cstring>

#include "living-tm.hpp"

#define M 2 //size of the alphabet
#define N 3 // number of states

using namespace std;


int main(int nArgs, char** args) {

  typedef living_tm<M, N> ltm;

  ltm t;
  unsigned long int nsteps;
  //  TSymbol symbol;
  //  TState state;
  //  TDirection d;

  uchar symbol;
  uchar state;
  char c_dir;

  bool running;

  if (nArgs == 3 && strcmp(args[1], "-f") == 0)
    t.load(args[2]);
  else if ((nArgs == 2) && (strcmp(args[1], "-i") == 0)) {
    
    cout << "Reading a Turing machine with " << M
	 << " symbols and " << N << " states...\n";
    
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < M; ++j) {
	cout << "State " << i << ", symbol " << j << " : ";
	cin >> symbol >> c_dir >> state;
	t.set_action(i, j, action<M, N>(state, symbol, c_dir == 'R' || c_dir == 'r'));
      }
    
    cout << endl << "[DONE]\n";

  } else if ((nArgs == 2) && (strcmp(args[1], "-h") == 0)) {
    cout << "This program execute a given Turing machine with " << M
	 << " symbols and " << N << " states.\n\n"
	 << "Usage :\n"
	 << "tur -f <filename>\tload a serialized machine\n"
	 << "tur -i \tinteractive input from stdin\n"
	 << "tur \tread machine from stdin with the format \
used when printing machines\n"
	 << "tur -h \tthis help page.\n";
    return EXIT_SUCCESS;
  }
  else {
    cerr << "Bad argument. Use -h for help." << endl;
    return EXIT_FAILURE;
  }
  
  do {
    cout << "The machine is :\n" << t << endl
	 << "How many steps (-1 for looping until the machine stops if so) ? ";
    cin >> nsteps;
    
    if (nsteps >= 0)
      running = t.do_nsteps(nsteps);
    else
      do {
	running = t.do_nsteps(100);
      }
      while (running);
    
  } while (running);
  
  cout << "[HALT]\n"
       << t
       << "\n\nThis machine stopped after "
       << t.get_nb_shifts() << endl;
  
  return EXIT_SUCCESS;
}
