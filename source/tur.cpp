#include <iostream>
#include "living-tm.hpp"
#include "limits.hpp"

#define M 2 //size of the alphabet
#define N 3 // number of states

using namespace std;


int main() {

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

  cout << "Reading a Turing machine with " << M
       << " symbols and " << N << " states...\n";
       
  
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < M; ++j) {
	cout << "State " << i << ", symbol " << j << " : ";
	cin >> symbol >> c_dir >> state;
	t.set_action(i, j, action<M, N>(state, symbol, c_dir == 'R' || c_dir == 'r'));
      }

  cout << endl << "[DONE]\n";


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
      
      return 0;
}
