/*****************************************************************************
 * tur.cpp                                                                   *
 * This program is aimed to make easier to run machines we find to ensure it *
 * runs in a certain number of steps. Machines are given as serialized       *
 * machines, via an interactive input or in the same format it is printed.   *
 *                                                                           *
 * /!\ Assumption is made that not M nor N are greater than 9 (because we    *
 * read it as a char, which cannot store more than one character, so covers  *
 * the range 0-9                                                             *
 *****************************************************************************/

#include <iostream>
#include "limits.hpp"
#include <cstring>
#include <cstdio>
#include "living-tm.hpp"
#include "nm.hpp" // change machine size in this file (M: alphabet, N: states)

#define LINE_LENGTH 100

using namespace std;

void parse_cell(char* s, uchar& symbol, uchar& state, char& c_dir) {
  // parse a cell of the transition table
  symbol = ((int) s[0]) - '0';
  c_dir = s[1];
  if (s[3] == 'H') state = N + 1;
  else state = s[3] - '0';
}

int main(int nArgs, char** args) {

  typedef living_tm<N, M> ltm;

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
    // read from file (serialized)
    t.load(args[2]);
  else if ((nArgs == 2) && (strcmp(args[1], "-i") == 0)) {
    // interactive (and probably boring) input

    cout << "Reading a Turing machine with " << M
	 << " symbols and " << N << " states...\n";
    
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < M; ++j) {
	cout << "State " << i << ", symbol " << j << " : ";
	cin >> symbol >> c_dir >> state;
	t.set_action(i, j, action<N, M>(state, symbol, c_dir == 'R' || c_dir == 'r'));
      }
    
    cout << endl << "[DONE]\n";

  } else if (nArgs == 1) {
    // read from stdin with the format of printed machines

    char s[LINE_LENGTH];

    // read the first (useless) line
    cin.ignore(LINE_LENGTH, '\n');
 
    for (int i = 0; i < N; ++i) {
      // for all line (state)

      // read the first (useless) column
      cin.ignore(LINE_LENGTH, ' ');

      for (int j = 0; j < M - 1; ++j) {
	// for all column (symbol)
	// except the last

	cin.getline(s, LINE_LENGTH, ' ');
	parse_cell(s, symbol, state, c_dir);
	t.set_action(i, j, action<N, M>(state, symbol, c_dir == 'R'));
      }
      // last colum :
	cin.getline(s, LINE_LENGTH);
	parse_cell(s, symbol, state, c_dir);
	t.set_action(i, M - 1, action<N, M>(state, symbol, c_dir == 'R'));
    }

  } else if ((nArgs == 2) && (strcmp(args[1], "-h") == 0)) {
    // prints help

    cout << "This program execute a given Turing machine with " << M
	 << " symbols and " << N << " states.\n\n"
	 << "Usage :\n"
	 << "tur -f <filename>\tload a serialized machine\n"
	 << "tur -i \tinteractive input from stdin\n"
	 << "tur \tread machine from stdin with the format \
used when printing machines\n"
	 << "tur -h \tthis help page.\n";
    return EXIT_SUCCESS;
  } else {
    cerr << "Bad argument. Use -h for help." << endl;
    return EXIT_FAILURE;
  }
  
  do {
    cout << "The machine is :\n" << t << endl
	 << "How many steps (-1 for looping until the machine stops if so) ? ";
    cin >> nsteps;
    cout << "OK, let's go for " << nsteps << " steps...\n";
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
       << t.get_nb_shifts() << " shifts.\n";
  
  return EXIT_SUCCESS;
}
